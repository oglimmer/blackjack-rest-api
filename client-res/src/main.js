
import {
    DefaultApi as _DefaultApi,
    CreatePlayerRequest as _CreatePlayerRequest,
    CreateGameRequest as _CreateGameRequest,
    BetRequest as _BetRequest,
    InsuranceRequest as _InsuranceRequest

} from "./index.js"

import {promisify} from "./promisify.js"
import ApiClient from "./ApiClient";

const DefaultApi = _DefaultApi;
const CreatePlayerRequest = _CreatePlayerRequest;
const CreateGameRequest = _CreateGameRequest;
const BetRequest = _BetRequest;
const InsuranceRequest = _InsuranceRequest;

const api = new DefaultApi(new ApiClient(window.location.protocol + "//" + window.location.host));

if(document.getElementById('highscore')) {
    (async () => {
        try {
            const highscore = await promisify(api.highscore.bind(api))();
            for(let i = 0; i < highscore.highscores.length; i++) {
                const s = highscore.highscores[i];
                document.getElementById('cell' + i + '_0').innerHTML = s.pos;
                document.getElementById('cell' + i + '_1').innerHTML = s.name;
                document.getElementById('cell' + i + '_2').innerHTML = s.money;
            }
        } catch (e) {
            console.log(e);
        }
    })();
}

if(document.getElementById('editor')) {
    let stop=false;
    document.getElementById('stop').addEventListener('click', async (event) => {
        stop=true;
        document.getElementById('start').disabled = false;
        document.getElementById('start100').disabled = false;
        document.getElementById('stop').disabled = true;
    });

    document.getElementById('start').addEventListener('click', async (event) => {
        document.getElementById('start').disabled = true;
        document.getElementById('start100').disabled = true;
        document.getElementById('stop').disabled = false;
        stop=false;
        document.getElementById('round').innerHTML = "(single)";
        await play({});
        document.getElementById('start').disabled = false;
        document.getElementById('start100').disabled = false;
        document.getElementById('stop').disabled = true;
    });

    document.getElementById('start100').addEventListener('click', async (event) => {
        document.getElementById('start').disabled = true;
        document.getElementById('start100').disabled = true;
        document.getElementById('stop').disabled = false;
        stop=false;
        const stats = {};
        for (let i = 0; i < 100 && !stop; i++) {
            document.getElementById('round').innerHTML = i;
            await play(stats);
        }
        document.getElementById('start').disabled = false;
        document.getElementById('start100').disabled = false;
        document.getElementById('stop').disabled = true;
    });

    async function play(stats) {
        const cashDiv = document.getElementById('cash');
        const infoDiv = document.getElementById('info');
        const statsDiv = document.getElementById('stats');

        const wrapper = new Function(editor.getValue()+ "; if (typeof begin === \"undefined\") { var begin; }; if (typeof end === \"undefined\") { var end; }; return [bet,command,insurance,result,begin,end];");

        if (wrapper()[4]) {
            wrapper()[4](stats);
        }

        const createPlayerRequest = new CreatePlayerRequest();
        const playerName = document.getElementById('playerName').value;
        if (playerName) {
            createPlayerRequest.name = playerName;
        }

        const playerResp = await promisify(api.createPlayer.bind(api))(createPlayerRequest);
        console.log(playerResp);
        infoDiv.innerHTML = JSON.stringify(playerResp);

        const deckResp = await promisify(api.createDrawDeck.bind(api))();
        console.log(deckResp);
        infoDiv.innerHTML = JSON.stringify(deckResp);

        let playerInfoResp = await promisify(api.getPlayer.bind(api))(playerResp.playerId);
        console.log(playerInfoResp);
        infoDiv.innerHTML = JSON.stringify(playerInfoResp);

        let maxMoney = playerInfoResp.cash;
        while(!stop && playerInfoResp.cash > 0) {
            const createGameRequest = new CreateGameRequest();
            createGameRequest.deckId = deckResp.deckId;
            let data = {
                yourCards: [],
                dealerCards: [],
                yourTotal: null,
                followActions: [],
                result: "in progress"
            };

            const gameResp = await promisify(api.createGame.bind(api))(createGameRequest);
            console.log(gameResp);
            infoDiv.innerHTML = JSON.stringify(gameResp);

            const betRequest = new BetRequest();
            betRequest.bet = wrapper()[0](playerInfoResp.cash, stats);
            betRequest.playerId = playerResp.playerId;

            const betResp = await promisify(api.placeBet.bind(api))(gameResp.gameId, betRequest);
            console.log(betResp);
            infoDiv.innerHTML = JSON.stringify(betResp);
            data.yourCards.push(betResp.card1);
            data.yourCards.push(betResp.card2);
            data.dealerCards.push(betResp.dealersCard);
            data.yourTotal = betResp.yourTotal;
            data.followActions = betResp.followActions;
            let betId = betResp.betId;
            let resp = betResp;

            if (resp.followActions.includes("insurance")) {
                const insuranceAnswer = wrapper()[2](data, stats);
                const insuranceRequest = new InsuranceRequest();
                insuranceRequest.insurance = insuranceAnswer ? "yes" : "no";
                resp = await promisify(api.insurance.bind(api))(gameResp.gameId, betId, insuranceRequest);
                console.log(resp);
                infoDiv.innerHTML = JSON.stringify(resp);
                data.followActions = resp.followActions;
            }

            let secondHand = null;

            while (resp.followActions.length > 0) {
                const cmdStr = wrapper()[1](data, stats);
                if (cmdStr == "hit") {
                    resp = await promisify(api.hit.bind(api))(gameResp.gameId, betId);
                    data.yourCards.push(resp.drawnCard);
                    data.yourTotal = resp.yourTotal;
                    data.followActions = resp.followActions;
                } else if (cmdStr == "stand") {
                    resp = await promisify(api.stand.bind(api))(gameResp.gameId, betId);
                    data.followActions = resp.followActions;
                } else if (cmdStr == "double") {
                    resp = await promisify(api.doubleBet.bind(api))(gameResp.gameId, betId);
                    data.yourCards.push(resp.drawnCard);
                    data.yourTotal = resp.yourTotal;
                    data.followActions = resp.followActions;
                } else if (cmdStr == "split") {
                    resp = await promisify(api.split.bind(api))(gameResp.gameId, betId);
                    data.yourCards = [];
                    data.yourCards.push(resp.firstBetCard1);
                    data.yourCards.push(resp.firstBetCard2);
                    data.yourTotal = resp.firstBetTotal;
                    data.followActions = resp.followActions;
                    secondHand = {
                        data: {
                            yourCards: [
                                resp.secondBetCard1,
                                resp.secondBetCard2
                            ],
                            dealerCards: data.dealerCards,
                            yourTotal: resp.secondBetTotal,
                            followActions: resp.secondBetFollowAction
                        },
                        betId: resp.secondBetId
                    }
                }
                console.log(resp);
                infoDiv.innerHTML = JSON.stringify(resp);


                const betResultResp = await promisify(api.getBet.bind(api))(gameResp.gameId, betId);
                console.log(betResultResp);
                infoDiv.innerHTML = JSON.stringify(betResultResp);
                data.dealerCards.push(betResultResp.dealersSecondCard);
                data.dealerCards.concat(betResultResp.dealersAdditionalCard);
                data.result = betResultResp.result;
                wrapper()[3](data, stats);
            }

            if(secondHand) {
                data = secondHand.data;
                betId = secondHand.betId;
                resp.followActions = secondHand.data.followActions;
                while (resp.followActions.length > 0) {
                    const cmdStr = wrapper()[1](data, stats);
                    if (cmdStr == "hit") {
                        resp = await promisify(api.hit.bind(api))(gameResp.gameId, betId);
                        data.yourCards.push(resp.drawnCard);
                        data.yourTotal = resp.yourTotal;
                        data.followActions = resp.followActions;
                    } else if (cmdStr == "stand") {
                        resp = await promisify(api.stand.bind(api))(gameResp.gameId, betId);
                        data.followActions = resp.followActions;
                    } else if (cmdStr == "double") {
                        resp = await promisify(api.doubleBet.bind(api))(gameResp.gameId, betId);
                        data.yourCards.push(resp.drawnCard);
                        data.yourTotal = resp.yourTotal;
                        data.followActions = resp.followActions;
                    } else if (cmdStr == "split") {
                        resp = await promisify(api.split.bind(api))(gameResp.gameId, betId);
                        data.yourCards = [];
                        data.yourCards.push(resp.firstBetCard1);
                        data.yourCards.push(resp.firstBetCard2);
                        data.yourTotal = resp.firstBetTotal;
                        data.followActions = resp.followActions;
                        secondHand = {
                            data: {
                                yourCards: [
                                    resp.secondBetCard1,
                                    resp.secondBetCard2
                                ],
                                yourTotal: resp.secondBetTotal,
                                followActions: resp.secondBetFollowAction
                            },
                            betId: resp.secondBetId
                        }
                    }
                    console.log(resp);
                    infoDiv.innerHTML = JSON.stringify(resp);


                    const betResultResp = await promisify(api.getBet.bind(api))(gameResp.gameId, betId);
                    console.log(betResultResp);
                    infoDiv.innerHTML = JSON.stringify(betResultResp);
                    data.dealerCards.push(betResultResp.dealersSecondCard);
                    data.dealerCards.concat(betResultResp.dealersAdditionalCard);
                    data.result = betResultResp.result;
                    wrapper()[3](data, stats);
                }
            }

            playerInfoResp = await promisify(api.getPlayer.bind(api))(playerResp.playerId);
            console.log(playerInfoResp);
            infoDiv.innerHTML = JSON.stringify(playerInfoResp);
            statsDiv.innerHTML = JSON.stringify(stats);
            cashDiv.innerHTML = playerInfoResp.cash;
            if(maxMoney < playerInfoResp.cash) {
                maxMoney = playerInfoResp.cash;
            }
        }
        cashDiv.innerHTML = "" + maxMoney + " (peak)";
        if (wrapper()[5]) {
            wrapper()[5](maxMoney, stats);
        }
        statsDiv.innerHTML = JSON.stringify(stats);
    }
}
