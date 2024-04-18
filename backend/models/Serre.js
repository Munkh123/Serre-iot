const {Schema, model} = require("mongoose");

const SerreSchema = new Schema({
    //nom
    name: {
        type: String,
        required: true
    },
    //serre
    number:{
        type: Number,
        required: true
    },
    tempHist:{
        type: [Number],
        required: true
    },
    humHist:{
        type: [Number],
        required: true
    },
    state: {
        type: String,
        required: true
    },
    temperature: {
        type: Number,
        required: true
    },
    humidity:{
        type: Number,
        required: true
    },
    //température idéale
    idealTemperature:{
        type: Number,
        required: true
    },
    //humidité idéale
    idealHumidity:{
        type: Number,
        required: true
    }
});

module.exports = model('Serre', SerreSchema);