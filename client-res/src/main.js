
import {
    DefaultApi as _DefaultApi,
    CreatePlayerRequest as _CreatePlayerRequest,
    CreateGameRequest as _CreateGameRequest,
    BetRequest as _BetRequest,
    InsuranceRequest as _InsuranceRequest

} from "./index.js"

import {promisify} from "./promisify.js"

const DefaultApi = _DefaultApi;
const CreatePlayerRequest = _CreatePlayerRequest;
const CreateGameRequest = _CreateGameRequest;
const BetRequest = _BetRequest;
const InsuranceRequest = _InsuranceRequest;

const api = new DefaultApi();

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
    });

    document.getElementById('start').addEventListener('click', async (event) => {

        const cashDiv = document.getElementById('cash');
        const infoDiv = document.getElementById('info');

        const wrapper = new Function(editor.getValue()+ "; return [bet,command,insurance];");

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
        stop=false;
        while(!stop && playerInfoResp.cash > 0) {
            const createGameRequest = new CreateGameRequest();
            createGameRequest.deckId = deckResp.deckId;

            const gameResp = await promisify(api.createGame.bind(api))(createGameRequest);
            console.log(gameResp);
            infoDiv.innerHTML = JSON.stringify(gameResp);

            const betRequest = new BetRequest();
            betRequest.bet = wrapper()[0](playerInfoResp.cash);
            betRequest.playerId = playerResp.playerId;

            const betResp = await promisify(api.placeBet.bind(api))(gameResp.gameId, betRequest);
            console.log(betResp);
            infoDiv.innerHTML = JSON.stringify(betResp);

            let resp = betResp;
            if (resp.followActions.includes("insurance")) {
                const insuranceAnswer = wrapper()[2](resp);
                const insuranceRequest = new InsuranceRequest();
                insuranceRequest.insurance = insuranceAnswer ? "yes" : "no";
                resp = await promisify(api.insurance.bind(api))(gameResp.gameId, betResp.betId, insuranceRequest);
                console.log(resp);
                infoDiv.innerHTML = JSON.stringify(resp);
            }

            while (resp.followActions.length > 0) {
                const cmdStr = wrapper()[1](resp);
                if (cmdStr == "HIT") {
                    resp = await promisify(api.hit.bind(api))(gameResp.gameId, betResp.betId);
                } else if (cmdStr == "STAND") {
                    resp = await promisify(api.stand.bind(api))(gameResp.gameId, betResp.betId);
                } else if (cmdStr == "DOUBLE") {
                    resp = await promisify(api.double.bind(api))(gameResp.gameId, betResp.betId);
                } else if (cmdStr == "SPLIT") {
                    resp = await promisify(api.split.bind(api))(gameResp.gameId, betResp.betId);
                }
                console.log(resp);
                infoDiv.innerHTML = JSON.stringify(resp);
            }

            const betResultResp = await promisify(api.getBet.bind(api))(gameResp.gameId, betResp.betId);
            console.log(betResultResp);
            infoDiv.innerHTML = JSON.stringify(betResultResp);

            playerInfoResp = await promisify(api.getPlayer.bind(api))(playerResp.playerId);
            console.log(playerInfoResp);
            infoDiv.innerHTML = JSON.stringify(playerInfoResp);
            cashDiv.innerHTML = playerInfoResp.cash;
            if(maxMoney < playerInfoResp.cash) {
                maxMoney = playerInfoResp.cash;
            }
        }
        cashDiv.innerHTML = "" + maxMoney + " (max)";
    });
}