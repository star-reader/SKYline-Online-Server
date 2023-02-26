import express from 'express'
import expressWs from 'express-ws'
import jwt from 'jsonwebtoken'
import key from '../../config/key/jwt'
import crypto from 'crypto'
import cryptoJs from 'crypto-js'
const router = express.Router()
const wsAp = expressWs(router as any)

const connections = new Map();

router.get('/',(req, res) => {
    res.send('hello')
})

//临时测试接口
router.get('/connect/token',(req, res) => {
    jwt.sign({
        cid: '2287',
        scope:['online','pilot','conteoller','authorization','view'],
        rating: 12,
        signature: crypto.randomBytes(36).toString('hex')
    },key, {'algorithm' : 'HS256' , 'expiresIn': '10days'}, (err, token) => {
      return res.send({
        code: 200,
        token,
        expire: 3600
      })  
    })
})

router.ws('/socketTest',(ws, req) => {
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
  ws.on('message',(data) => {
    console.log(req.headers.host)
    ws.send('hhhhh')
  })
  
})

export default router