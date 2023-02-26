import express from 'express'
import expressWs from 'express-ws'
import cors from 'cors'
import bodyParser from 'body-parser'
import router from './src/router/index'

const app = express()
expressWs(app)
app.use(cors())
app.use(bodyParser.json())
app.use('/online', router)


app.listen(8010, () => {
    console.log(`server is running on port 8010`)
})