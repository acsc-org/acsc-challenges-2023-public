const Fastify = require('fastify')
const proxy = require('@fastify/http-proxy')

const server = Fastify({ logger: true })

server.register(proxy, {
  upstream: 'http://app:3001',
  replyOptions: {
    rewriteRequestHeaders: (req, headers) => {
      const allowedHeaders = [
        'host', 'user-agent', 'accept', 'template'
      ]
      return Object.fromEntries(Object.entries(headers).filter(el => allowedHeaders.includes(el[0])))
    },
    onResponse: (request, reply, res) => {
      const dataChunk = []
      res.on('data', chunk => {
        dataChunk.push(chunk)
      })

      res.on('end', () => {
        const data = dataChunk.join('')

        if (/ACSC\{.*\}/.test(data)) {
          return reply.code(403).send("??")
        }

        return reply.send(data)
      })
    },
  }
})

server.listen({ host: '0.0.0.0', port: 3000 })
