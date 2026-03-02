Comment créez et entrainer votre réseau de neurones...?
Allez à la racine du répertoire, faites
Ensuite allez dans le dossier scripts : vous y trouverez deux binaires : 
generate_NN_config.py et generate_training_config.py
Faites : python3 generate_NN_config.py
Remplissez les informations...
Le fichier est automatiquement créé dans le dossier NN_Config à la racine du répertoire sous le nom que vous avez indiqué
Pour nommer votre fichier vous utiliserez la convention : 
<nombre de neurones couches n>-<nombre de neurones couches n + 1>...<fonction activation hidden>-<fonction activation sortie>.json
Exemple : 164-34-43-RELU-SIGMOID.json (vous pouvez interpreter)
Ensuite, lancez ./bin/shell depuis la racine du repertoire. Le shell va se lancer
Tapez la commande GENERATE <votre-fichier-de-config>
Félicitations vous venez de créer votre réseau de neurones
N'oubliez pas de le sauvegarder en faisant SAVE <nom>
La définition du nom respecte la même convention que définie precedemment sauf qu'on ne mets pas le .json à la fin
Maintenant il faut entrainer votre réseau de neurones
Retournez dans le dossier script et lancez le deuxième script. Remplissez les informations. Passez le nom exact de votre modèle quand on vous le demande. Pour le nom du fichier, donnez 'training<n>.json'
n = 0 si c'est votre premier entrainement et ainsi de suite..
le fichier est automatiquement sauvegardé dans NN_Training_Config/<nom_de_votre_modele>
Retournez dans le shell et faites TRAIN <chemin-vers-le-fichier-de-config><chemin-vers-le-fichier-d'entrainement>
Les fichiers d'entrainement sont situés dans training_data/
Après l'entrainement de votre modèle, sauvegardez à nouveau..
Pour évaluer les métriques de votre modèle, faites 'evaluate <nom-de-fichier>
Les fichiers de tests sont dans testing_data...
Vous aurez les métriques, a toi Jordan de t'en servir...
Pour prédire avec votre modèle, faites predict <nom-de-fichier>
Pour le genre de fichiers referrez vous au pdf du projet avec l'example sur my_torch_analyzer

NB: N'oubliez pas de sauvegarder en quittant le shell... Et a chaque fois que vous rouvrez le shell, loadez votre network d'abord en faisant 'load <nom-de-votre-modele>'