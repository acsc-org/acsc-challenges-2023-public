const pptr = require('puppeteer')
const net = require('net')

const APP_BASEURL = process.env.APP_BASEURL || 'http://nginx/'
const APP_DOMAIN = (new URL(APP_BASEURL)).host
const FLAG = process.env.FLAG || 'CTF{dummyflag}'
const BROWSER_POOL = process.env.BROWSER_POOL || 4

const sleep = d => new Promise(r => setTimeout(r, d));

class CralwQueue {
  browsers = []
  isBrowserAvailable = new Map()
  queue = []

  constructor(browserPoolSize) {
    // launch browsers
    for (let i = 0; i < browserPoolSize; ++i) {
      this.launchBrowser()
    }

    this.loop()
  }

  add(path) {
    this.queue.push(path)
  }

  async launchBrowser() {
    const browser = await pptr.launch({
      args: ["--no-sandbox"]
    })
    this.browsers.push(browser)
    this.isBrowserAvailable.set(browser, true)
  }

  async loop() {
    this.browsers.forEach((browser) => {
      if (!this.isBrowserAvailable.get(browser)) {
        return
      }
      if (this.queue.length === 0) {
        return
      }

      const queue = this.queue.shift()
      this.isBrowserAvailable.set(browser, false)
      console.log(`occupy browser: ${[...this.isBrowserAvailable.values()]}`)
      this.crawl(browser, queue).finally(() => {
        this.isBrowserAvailable.set(browser, true)
      })
    })

    await sleep(100)
    this.loop()
  }

  async crawl(browser, path = '') {
    let url = new URL(APP_BASEURL)
    url.pathname = path
    url = url.toString()

    console.log("crawl start:", url)

    const context = await browser.createIncognitoBrowserContext()

    // create page
    const page = await context.newPage()

    await page.setCookie({
      name: "FLAG",
      value: FLAG,
      domain: APP_DOMAIN,
      path: "/",
    });

    try {
      await page.goto(url, {
        timeout: 2000,
      });
      await sleep(2000);
    } catch (e) {
      console.log('error', e);
    }

    await page.close();
    await context.close();

    console.log("crawl end:", url)
  }
}

const main = async () => {
  const queue = new CralwQueue(BROWSER_POOL)
  const server = net.createServer((socket) => {
    socket.on('data', (data) => {
      const path = data.toString().trim()
      queue.add(path)
    })
  })
  server.listen(3000, '0.0.0.0')
}

main();
