const express = require('express');
const app = express();

app.set('view engine', 'ejs');

const path = require('path');
const staticPath = path.join(__dirname, "./public");
app.use(express.static(staticPath));

const mqtt = require('mqtt');
const client = mqtt.connect('mqtt://test.mosquitto.org');

client.on('connect', () => {
  client.subscribe('datosFocoEsli');
});

/*
client.on('message', (topic, message) => {
  console.log(`Topic[${topic}]: ${message}`);
});
*/
app.get('/flama', (req, res) => {
  const data = {
    nombre: "Esteban"
  }
  res.render('indexRender', { data })
})

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index3.html');
})

app.get('/publish', (req, res) => {
  client.publish('focoEsli', 'Hola ESLI');
  console.log('publish');
  res.status(200);
  res.json("Msj")
})


app.listen(3000);