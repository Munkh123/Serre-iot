import React, {useEffect, useState} from 'react';
import {Link} from "react-router-dom";
import "./pages/styles/App.css"

/*Classe pour afficher toutes les serres à l'accueil*/
function App(){
    const [hotHouses, setHotHouses] = useState([{_id:"1203",name:"", number:"", state:""}]);

    useEffect(() =>{
        //récupération des serres
        async function fetchAll(){
            try{
                const reponse = await fetch(`http://localhost:3001/serre/serres/all`);
                if (reponse.ok){
                    return await reponse.json()
                }else{
                    console.error(`Fetch des serres échoué. Erreur: ${reponse.status}`);
                }
            } catch (error) {
                console.error("Erreur fetch des serres:", error);
            }
        }
        fetchAll().then(data => setHotHouses(data));
    }, []);


    return (
        <div className={"page"}>
            <h1 className={"titre"}>Liste des serres</h1>
            <div className={"listSerres"}>
                {hotHouses.map(plant => (
                    <div key={plant._id} className={"serre"}>
                        <div className={"titreCarte"}>
                            <div className={"linkCarte"}><Link className={"link"}
                                                               to={`/${plant.number}`}>{plant.name}</Link></div>
                            <div className={"circleHome"} style={{background: plant.state}}></div>
                        </div>
                        <a href={`/${plant.number}`}>
                            <img src={`/images/${plant.name.trim()}.jpg`} alt="default"/> </a>
                    </div>
                ))}
            </div>
        </div>
    );
}

export default App;