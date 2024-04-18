FROM node:alpine

WORKDIR /app

RUN npm install serialport

COPY package*.json ./

RUN npm install

COPY . ./

EXPOSE 3001

CMD [ "npm", "start" ]
