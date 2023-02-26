"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const express_ws_1 = __importDefault(require("express-ws"));
const cors_1 = __importDefault(require("cors"));
const body_parser_1 = __importDefault(require("body-parser"));
const index_1 = __importDefault(require("./src/router/index"));
const app = (0, express_1.default)();
(0, express_ws_1.default)(app);
app.use((0, cors_1.default)());
app.use(body_parser_1.default.json());
app.use('/online', index_1.default);
app.listen(8010, () => {
    console.log(`server is running on port 8010`);
});
