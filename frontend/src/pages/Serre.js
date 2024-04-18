import React, {useEffect, useState} from "react";
import "./styles/Serre.css"
import { useParams, useNavigate } from "react-router-dom";

/*Classe pour afficher les détails d'une serre*/

function Serre(){
    const { number } = useParams();
    const [plant, setPlant] = useState({});
    const [avgTemp, setAvgTemp] = useState("");
    const [avgHum, setAvgHum] = useState("");
    let navigate = useNavigate();


    useEffect(() => {
        //récupération par numéro d'une serre
        async function fetchSerre() {
            try {
                const reponse = await fetch(`http://localhost:3001/serre/serres/${number}`);
                if (reponse.ok) {
                    const data = await reponse.json();
                    setPlant(data);
                    //formatage et calcul de la température et humidité moyenne.
                    setAvgTemp((data.tempHist.reduce((a, b) => a + b, 0) / data.tempHist.length).toFixed(2));
                    setAvgHum((data.humHist.reduce((a, b) => a + b, 0) / data.humHist.length).toFixed(2));
                }
            } catch (e) {
                console.error("Erreur fetch de serre par numéro:", e);
            }
        }
        fetchSerre().then(r => console.log("fetch serrre par numéro!!"));
    }, [number]);


    function Plant(){
        return (
            <div className={"plantDetails"}>
                <h1 className={"titre"}>Serre : {plant.number}</h1>
                <button className={"retourBouton"} onClick={() => navigate(-1)}>Retour</button>
                <div className="carte">
                    <div className="containerCarte">
                        <h2 className={"namePlant"}><b>{plant.name}</b></h2>
                        <div className={"rowCarte"}>
                            <p className={"titreValue"}>état : </p>
                            <p className={"circle"} style={{background: plant.state}}></p>
                        </div>
                        <div className={"rowCarte"}>
                            <p className={"titreValue"}>température moyenne : </p>
                            <p className={"valueCarte"}>{avgTemp} °C</p>
                        </div>
                        <div className={"rowCarte"}>
                            <p className={"titreValue"}>humidité moyenne : </p>
                            <p className={"valueCarte"}>{avgHum} %</p>
                        </div>
                        <div className={"rowCarte"}>
                            <p className={"titreValue"}>température :</p>
                            <p className={"valueCarte"}>{plant.temperature} °C</p>
                        </div>
                        <div className={"rowCarte"}>
                            <p className={"titreValue"}>humidité :</p>
                            <p className={"valueCarte"}>{plant.humidity} %</p>
                        </div>
                        <div className={"rowCarte"}>
                            <p className={"titreValue"}>humidité idéale :</p>
                            <p className={"valueCarte"}>{plant.idealHumidity} %</p>
                        </div>
                        <div className={"rowCarte"}>
                            <p className={"titreValue"}>température idéale :</p>
                            <p className={"valueCarte"}>{plant.idealTemperature} °C</p>
                        </div>
                    </div>
                </div>
            </div>
        );
    }

    return (
        <div className={"parent"}>
            <Plant></Plant>
        </div>
    );
}

export default Serre;