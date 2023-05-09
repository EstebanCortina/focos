const express = require('express');
const app = express();

const mqtt = require('mqtt');
const client = mqtt.connect('mqtt://test.mosquitto.org');

client.on('connect', () => {
  console.log("asd");
});


app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
})

app.post('/', (req, res) => {
  client.publish('focoEsli', 'Hola ESLI');
  res.status(200);
  res.redirect('/');
})


app.listen(3000);