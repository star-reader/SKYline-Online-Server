"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const express_ws_1 = __importDefault(require("express-ws"));
const jsonwebtoken_1 = __importDefault(require("jsonwebtoken"));
const jwt_1 = __importDefault(require("../../config/key/jwt"));
const crypto_1 = __importDefault(require("crypto"));
const router = express_1.default.Router();
const wsAp = (0, express_ws_1.default)(router);
const connections = new Map();
router.get('/', (req, res) => {
    res.send('hello');
});
//临时测试接口
router.get('/connect/token', (req, res) => {
    jsonwebtoken_1.default.sign({
        cid: '2287',
        scope: ['online', 'pilot', 'conteoller', 'authorization', 'view'],
        rating: 12,
        signature: crypto_1.default.randomBytes(36).toString('hex')
    }, jwt_1.default, { 'algorithm': 'HS256', 'expiresIn': '10days' }, (err, token) => {
        return res.send({
            code: 200,
            token,
            expire: 3600
        });
    });
});
router.ws('/socketTest', (ws, req) => {
    // ws.on('message',(data) => {
    //   connections.set(ws, req.query.title)
    //   connections.forEach((value, key) => {
    //     if (value === req.query.very){
    //       //是消息的接受者
    //       console.log('yes')
    //       key.send(value)
    //     }
    //   })
    // })
    ws.on('message', (data) => {
        console.log(req.headers.host);
        ws.send('hhhhh');
    });
});
exports.default = router;
