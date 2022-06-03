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

if (document.getElementById('highscore')) {
    (async () => {
        try {
            const highscore = await promisify(api.highscore.bind(api))();
            for (let i = 0; i < highscore.highscores.length; i++) {
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

function buttons(disabled) {
    document.getElementById('start').disabled = disabled;
    document.getElementById('start100').disabled = disabled;
    document.getElementById('startForever').disabled = disabled;
    document.getElementById('stop').disabled = !disabled;
}

if (document.getElementById('editor')) {
    let stop = false;
    document.getElementById('stop').addEventListener('click', async (event) => {
        stop = true;
        buttons(false);
    });

    document.getElementById('start').addEventListener('click', async (event) => {
        buttons(true);
        stop = false;
        document.getElementById('round').innerHTML = "(single)";
        await play({});
        buttons(false);
    });

    document.getElementById('start100').addEventListener('click', async (event) => {
        buttons(true);
        stop = false;
        const stats = {};
        for (let i = 0; i < 100 && !stop; i++) {
            document.getElementById('round').innerHTML = i;
            await play(stats);
        }
        buttons(false);
    });

    document.getElementById('startForever').addEventListener('click', async (event) => {
        buttons(true);
        stop = false;
        const stats = {};
        let i = 0;
        while (!stop) {
            document.getElementById('round').innerHTML = i++;
            await play(stats);
        }
    });

    const INDEX_BET = 0;
    const INDEX_COMMAND = 1;
    const INDEX_INSURANCE = 2;
    const INDEX_RESULT = 3;
    const INDEX_BEGIN = 4;
    const INDEX_CASHUPDATE = 5;
    const INDEX_END = 6;

    async function processCommand(resp, data, betId, infoDiv, wrapper, stats, gameResp, hand2nd) {
        let secondHand = null;
        try {
            while (resp.followActions.length > 0) {
                const cmdStr = wrapper()[INDEX_COMMAND](data, stats, hand2nd);
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
                    if (hand2nd) throw "hand2nd";
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
            }
        } catch (e) {
            console.error(e);
        }
        const betResultResp = await promisify(api.getBet.bind(api))(gameResp.gameId, betId);
        console.log(betResultResp);
        infoDiv.innerHTML = JSON.stringify(betResultResp);
        if (betResultResp.dealersSecondCard) {
            data.dealerCards.push(betResultResp.dealersSecondCard);
        }
        if (betResultResp.dealersAdditionalCard.length > 0) {
            data.dealerCards.concat(betResultResp.dealersAdditionalCard);
        }
        data.result = betResultResp.result;
        data.dealerTotal = betResultResp.dealerTotal;
        data.payout = betResultResp.payout;
        const feedbackResult = wrapper()[INDEX_RESULT](data, stats);

        return {
            secondHand,
            resp,
            feedbackResult
        }
    }

    async function play(stats) {
        const cashDiv = document.getElementById('cash');
        const infoDiv = document.getElementById('info');
        const statsDiv = document.getElementById('stats');

        const wrapper = new Function(editor.getValue() + "; " +
            "if (typeof bet === \"undefined\") { var bet = () => {return 1;}; }; " +
            "if (typeof command === \"undefined\") { var command = () => {return \"stand\";}; }; " +
            "if (typeof insurance === \"undefined\") { var insurance = () => {return false;}; }; " +
            "if (typeof result === \"undefined\") { var result = () => {}; }; " +
            "if (typeof begin === \"undefined\") { var begin = () => {}; }; " +
            "if (typeof cashUpdate === \"undefined\") { var cashUpdate = () => {}; }; " +
            "if (typeof end === \"undefined\") { var end = () => {}; }; " +
            "return [bet,command,insurance,result,begin,cashUpdate,end];");

        wrapper()[INDEX_BEGIN](stats);

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

        let takeMoneyAtStopThisPlay = false;
        let maxMoney = playerInfoResp.cash;
        while (!stop && playerInfoResp.cash > 0 && !takeMoneyAtStopThisPlay) {
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
            betRequest.bet = wrapper()[INDEX_BET](playerInfoResp.cash, stats);
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
                const insuranceAnswer = wrapper()[INDEX_INSURANCE](data, stats);
                const insuranceRequest = new InsuranceRequest();
                insuranceRequest.insurance = insuranceAnswer ? "yes" : "no";
                resp = await promisify(api.insurance.bind(api))(gameResp.gameId, betId, insuranceRequest);
                console.log(resp);
                infoDiv.innerHTML = JSON.stringify(resp);
                data.followActions = resp.followActions;
            }

            let secondHand = null, feedbackResult = null;
            ({secondHand, resp, feedbackResult} = await processCommand(resp, data, betId, infoDiv, wrapper, stats, gameResp, false));
            if (feedbackResult===true) {
                takeMoneyAtStopThisPlay = true;
            }

            if (secondHand) {
                data = secondHand.data;
                betId = secondHand.betId;
                resp.followActions = secondHand.data.followActions;
                ({resp, feedbackResult} = await processCommand(resp, data, betId, infoDiv, wrapper, stats, gameResp, true));
                if (feedbackResult===true) {
                    takeMoneyAtStopThisPlay = true;
                }
            }

            playerInfoResp = await promisify(api.getPlayer.bind(api))(playerResp.playerId);
            console.log(playerInfoResp);
            infoDiv.innerHTML = JSON.stringify(playerInfoResp);
            statsDiv.innerHTML = JSON.stringify(stats);
            cashDiv.innerHTML = playerInfoResp.cash;
            if (maxMoney < playerInfoResp.cash) {
                maxMoney = playerInfoResp.cash;
            }
            feedbackResult = wrapper()[INDEX_CASHUPDATE](playerInfoResp.cash, stats);
            if (feedbackResult===true) {
                takeMoneyAtStopThisPlay = true;
            }
        }
        cashDiv.innerHTML = "" + maxMoney + " (peak)";
        wrapper()[INDEX_END](playerInfoResp.cash, maxMoney, stats);
        statsDiv.innerHTML = JSON.stringify(stats);
    }
}
