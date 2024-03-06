FROM node

RUN mkdir /proxy
WORKDIR /proxy
ADD package.json .
RUN npm install

ADD index.js .

ENTRYPOINT ["npm", "run", "start"]
EXPOSE 3000
