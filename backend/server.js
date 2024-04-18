const express = require("express")
const app = express()
const port = 3001
const Serre  = require("./models/Serre");
const mongoose = require('mongoose');
const cors = require("cors")
app.use(cors())

//mongodb connexion
mongoose.connect('mongodb://mongodb:27017/', {
  dbName: "serre",
  useNewUrlParser: true,
  useUnifiedTopology: true
}).then(r => console.log("mongodb connecté!!!")).catch(e=>console.log(`CONNEXION ECHOUE: ${e}`));

//création des schéma dans la bd
Serre.createIndexes().then(r => console.log("Serre created to mongoDB!!!"));

//données pour test
insert([ "carotte",10,"green",0.00,74.00,20,50]).then(r => console.log( `test data insertion: carotte`));
insert([ "carotte",8,"green",0.00,74.00,20,50]).then(r => console.log( `test data insertion: carotte`));
insert([ "carotte",6,"red",0.00,74.00,20,50]).then(r => console.log( `test data insertion: carotte`));
insert([ "carotte",3,"blue",0.00,74.00,20,50]).then(r => console.log( `test data insertion: carotte`));

//arduino
const { SerialPort, ReadlineParser } = require('serialport')
const portArduino = new SerialPort({ path:"/dev/ttyACM0", baudRate:9600 })
const parser = new ReadlineParser({delimiter:"\n"})
portArduino.pipe(parser)
//écoute les données
parser.on('data', function (data) {
  if(data.includes("details :")) {
    const fields = data.split(":")[1].split("|");
    //console.log(`name:${fields[0]}, number:${fields[1]}, state:${fields[2]}, temperature:${fields[3]}, humidity:${fields[4]},idealTemperature:${fields[5]},idealHumidity:${fields[6]}`)
    insert(fields).then(r => console.log( `new DATA: ${data}`));
  }
});

//méthode pour insertion de données ou maj
async function insert(tabData)  {
  console.log(`Données à insérer:${tabData}`);
  await Serre.findOne({ number: tabData[1] })
      .then(result => {
        if (result) {
          console.log(tabData[1]," existe déjà, maj des données");
          Serre.updateOne({ number: tabData[1] }, { $set: {state: tabData[2],temperature:tabData[3], humidity: tabData[4], tempHist:[tabData[3], ...result.tempHist], humHist:[tabData[4], ...result.humHist]} })
              .then(result => {
              })
              .catch(error => {
                console.error(error);
              });
        } else {
          console.log(tabData[1]," n'existe pas, insertion");
          const newSerre = new Serre({name:tabData[0].trim(), number:tabData[1], tempHist:[tabData[3]], humHist:[tabData[4]], state:tabData[2],temperature:tabData[3], humidity:tabData[4], idealTemperature:tabData[5], idealHumidity: tabData[6]});
          newSerre.save();
        }
      })
      .catch(err => {
        console.error(err);
      });
}

//récupération de toutes les serres
app.get("/serre/serres/all", async (req, res) => {
  try {
    const serres = await Serre.find();
    res.status(200).json(serres);
  } catch (error) {
    console.error(error);
    res.status(500).json({ message: "erreur récupération de toutes les données" });
  }
});

//récupération d'une serre par number
app.get("/serre/serres/:number", async (req, res) => {
  const serreNumber = req.params.number;
  try {
    const serre = await Serre.findOne({number:serreNumber});
    if (!serre) {
      return res.status(404).json({ message: "Serre non trouvée" });
    }

    res.status(200).json(serre);
  } catch (error) {
    console.error(error);
    res.status(500).json({ message: "erreur de récupération par number" });
  }
});

app.listen(port, () => {
  console.log(`Serveur est en cours sur le port: ${port}`)
})