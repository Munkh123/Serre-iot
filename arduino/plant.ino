#include <DHT_Async.h>

#define DHT_SENSOR_PIN 2
#define DHT_SENSOR_TYPE DHT_TYPE_11
#define ROUGE 3
#define VERT 5
#define BLEU 6
DHT_Async dht_capteur(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
// Structure définissant les informations d'une plante
struct PlanteInfo {  
  //int numeroPlante;
  const char* nomAffiche;
  float idealTemperatureMin;
  float idealTemperatureMax;
  float idealHumidityMin;
  float idealHumidityMax;
};

// Tableau contenant les informations sur différentes plantes
PlanteInfo plantesData[] = {
  {"Plante1", 15, 25, 40, 60},
  {"Plante2", 18, 28, 45, 65},
  {"Plante3", 18, 28, 45, 65},
  {"Plante4", 19, 35, 45, 65},
  {"Plante5", 18, 22, 45, 65},
  {"Plante6", 18, 23, 45, 65},
  {"PlanteFroide", 2, 10, 50, 70},
  {"PlanteChaude", 21, 35, 20, 40},
};

// Chaînes de caractères pour stocker les plantes adaptées et non adaptées
String plantesAdaptees = "";
String plantesNonAdaptees = "";
String currentColor="";

// Fonction d'initialisation
void setup() {
  Serial.begin(9600);

  // Configuration des broches pour les LEDs RGB
  pinMode(ROUGE, OUTPUT);
  pinMode(VERT, OUTPUT);
  pinMode(BLEU, OUTPUT);

  // Définition de la couleur par défaut
  definirCouleur(255, 0, 0);
}

void loop() {
  float temperature, humidite;


  // Mesurer la température et l'humidité de l'environnement
  mesurerEnvironnement(&temperature, &humidite);
  Serial.println("details : tomate|4|"+currentColor+"|"+String(temperature)+ "|"+String(humidite)+"|"+ 20+"|"+ 50);
  // Obtenir les plantes adaptées aux conditions mesurées
  obtenirPlantesAdaptees(temperature, humidite);
  // Afficher les plantes adaptées et non adaptées
  afficherPlantesAdaptees();

  // Trouver les conditions idéales pour la Plante1 et ajuster la couleur en conséquence
  trouverPlante("Plante1",temperature, humidite);

  delay(15000);
}

// Fonction pour trouver la plante passer en paramètre et ajuster la couleur en conséquence
void trouverPlante(const char* plante, float temperature, float humidite) {
  // Récupérer les conditions idéales pour la plante passer en paramètre
  PlanteInfo* plante1 = recupererConditionsIdeales(plante);

  // Ajuster la couleur en fonction des conditions mesurées
  if (temperature < plante1->idealTemperatureMin || humidite > plante1->idealHumidityMax) {
    definirCouleur(0, 0, 255);// Bleu si en dessous des conditions minimales
  } else if (temperature > plante1->idealTemperatureMax || humidite < plante1->idealHumidityMin) {
    definirCouleur(255, 0, 0);// Rouge si au-dessus des conditions maximales
  } else if (temperature < plante1->idealTemperatureMax && temperature > plante1->idealTemperatureMin &&
             humidite < plante1->idealHumidityMax && humidite > plante1->idealHumidityMin) {
    definirCouleur(0, 255, 0);// Vert si dans les conditions idéales
  }

  // Vérifier si la Plante1 correspond aux conditions actuelles
  if (plante1 != nullptr && conditionPlanteIdeale(*plante1, temperature, humidite)) {
    Serial.print("La Plante : ");
    Serial.print(plante);
    Serial.println(" correspond aux conditions actuelles.");
  } else {
    Serial.print("La Plante : ");
    Serial.print(plante);
    Serial.println(" ne correspond pas aux conditions actuelles.");
  }
}

// Fonction pour mesurer la température et l'humidité de l'environnement
bool mesurerEnvironnement(float *temperature, float *humidite) {
  static unsigned long horodatageMesure = millis();

  // Mesurer toutes les 3 secondes pour éviter de surcharger le capteur
  if (millis() - horodatageMesure > 3000ul) {
    if (dht_capteur.measure(temperature, humidite)) {
      horodatageMesure = millis();
      return true;
    }
  }

  return false;
}

// Fonction pour récupérer les conditions idéales d'une plante donnée
PlanteInfo* recupererConditionsIdeales(const char *typePlante) {
  for (size_t i = 0; i < sizeof(plantesData) / sizeof(plantesData[0]); ++i) {
    if (strcmp(plantesData[i].nomAffiche, typePlante) == 0) {
      return &plantesData[i];
    }
  }
  return nullptr;
}

// Fonction pour ajouter une plante adaptée ou non adaptée à la chaîne de caractères correspondante
void ajouterPlanteAdaptee(const char *typePlante, float temperature, float humidite, PlanteInfo *plantesData, size_t taille) {
  PlanteInfo* plante = recupererConditionsIdeales(typePlante);
  if (plante != nullptr && conditionPlanteIdeale(*plante, temperature, humidite)) {
    plantesAdaptees += typePlante;
    plantesAdaptees += ",";
  } else {
    plantesNonAdaptees += typePlante;
    plantesNonAdaptees += ",";
  }
}

// Fonction pour afficher une plante correspondante aux conditions actuelles
void afficherPlantesCorrespondantes(const char *typePlante, float temperature, float humidite, PlanteInfo *plantesData, size_t taille) {
  PlanteInfo* plante = recupererConditionsIdeales(typePlante);
  if (plante != nullptr && conditionPlanteIdeale(*plante, temperature, humidite)) {
    /*Serial.print("La plante ");
    Serial.print(typePlante);
    Serial.println(" correspond aux conditions actuelles.");*/
  }
}

// Fonction pour vérifier si les conditions d'une plante sont idéales
bool conditionPlanteIdeale(const PlanteInfo& plante, float temperature, float humidite) {
  return (temperature >= plante.idealTemperatureMin && temperature <= plante.idealTemperatureMax &&
          humidite >= plante.idealHumidityMin && humidite <= plante.idealHumidityMax);
}

// Fonction pour afficher si une plante correspond aux conditions actuelles
void afficherPlantesCorrespondantes(const char *typePlante, float temperature, float humidite) {
  // Récupérer les conditions idéales de la plante spécifiée
  PlanteInfo* plante = recupererConditionsIdeales(typePlante);
  
  // Vérifier si la plante existe et si ses conditions sont idéales
  if (plante != nullptr && conditionPlanteIdeale(*plante, temperature, humidite)) {
    // Afficher le message indiquant que la plante correspond aux conditions actuelles
    /*Serial.print("La plante ");
    Serial.print(typePlante);
    Serial.println(" correspond aux conditions actuelles.");*/
  }
}

// Fonction pour traiter les plantes en fonction du type de traitement spécifié
void traiterPlantes(const char *typeTraitement, float temperature, float humidite, PlanteInfo *plantesData, size_t taille) {
  // Chaîne de caractères pour stocker les noms des plantes disponibles
  String plantesDisponibles = "";
  
  // Construire la chaîne avec les noms des plantes disponibles
  for (size_t i = 0; i < taille; ++i) {
    plantesDisponibles += plantesData[i].nomAffiche;
    plantesDisponibles += ",";
  }

  // Variables pour parcourir la chaîne de caractères des noms de plantes
  int indiceDebut = 0;
  int indiceFin = plantesDisponibles.indexOf(',');
  String plante;

  // Parcourir la chaîne de caractères des noms de plantes
  while (indiceFin != -1) {
    // Extraire le nom de la plante de la sous-chaîne
    plante = plantesDisponibles.substring(indiceDebut, indiceFin);
    
    // Effectuer le traitement en fonction du type spécifié
    if (strcmp(typeTraitement, "adaptees") == 0) {
      ajouterPlanteAdaptee(plante.c_str(), temperature, humidite, plantesData, taille);
    } else if (strcmp(typeTraitement, "correspondantes") == 0) {
      afficherPlantesCorrespondantes(plante.c_str(), temperature, humidite, plantesData, taille);
    }
    
    // Mettre à jour les indices pour la prochaine itération
    indiceDebut = indiceFin + 1;
    indiceFin = plantesDisponibles.indexOf(',', indiceDebut);
  }

  // Traitement de la dernière plante dans la chaîne
  plante = plantesDisponibles.substring(indiceDebut);
  if (strcmp(typeTraitement, "adaptees") == 0) {
    ajouterPlanteAdaptee(plante.c_str(), temperature, humidite, plantesData, taille);
  } else if (strcmp(typeTraitement, "correspondantes") == 0) {
    afficherPlantesCorrespondantes(plante.c_str(), temperature, humidite, plantesData, taille);
  }
}

// Fonction pour obtenir les plantes adaptées aux conditions spécifiées
void obtenirPlantesAdaptees(float temperature, float humidite) {
  traiterPlantes("adaptees", temperature, humidite, plantesData, sizeof(plantesData) / sizeof(plantesData[0]));
}

// Fonction pour obtenir les plantes correspondant aux conditions spécifiées
void obtenirPlantesCorrespondantes(float temperature, float humidite) {
  traiterPlantes("correspondantes", temperature, humidite, plantesData, sizeof(plantesData) / sizeof(plantesData[0]));
}


// Fonction pour définir la couleur des LEDs RGB
void definirCouleur(uint8_t rouge, uint8_t vert, uint8_t bleu) {
  analogWrite(ROUGE, rouge);
  analogWrite(VERT, vert);
  analogWrite(BLEU, bleu);

  if(bleu >0){
    currentColor="blue";
  }
  else if(vert >0){
    currentColor="green";
  }
  else{
    currentColor="red";
  }

}

// Fonction pour afficher les plantes adaptées et non adaptées
void afficherPlantesAdaptees() {
  if (plantesAdaptees.length() > 0) {
    plantesAdaptees.remove(plantesAdaptees.length() - 1);
    Serial.println("plantsAdaptees:["+plantesAdaptees+"]");
  } else {
    Serial.println("plantsAdaptees:[]");
  }

  //Serial.println("Liste des plantes non adaptées :");
  if (plantesNonAdaptees.length() > 0) {
    plantesNonAdaptees.remove(plantesNonAdaptees.length() - 1);
    //Serial.println(plantesNonAdaptees);
  } else {
    //Serial.println("Aucune plante non adaptée.");
  }

  plantesAdaptees = "";
  plantesNonAdaptees = "";
}