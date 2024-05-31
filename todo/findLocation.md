# But

Une fonction findRoot qui permet de matcher un path de block location (du fichier de conf) avec le path d'une requete

# Input

```C++
std::string findLocation(std::map<std::string, Route> route, const std::string& requestPath); 
```

# Valeur de retour

A definir mais potentiellement simplement la cle de la map (string) et string vide si pas de match

# Contexte

Si la request est 

```
GET /fichier
```

```
location /fichier {
	root /var/www
}
```

on utilisera la path /var/www/fichier

## Cas tricky

Si requete

```
GET /dossier/dossier2/file
```

```
location /dossier/dossier2 {
	root /var
}

location /dossier {
	root /etc
}
```

la path sera /var/dossier/dossier2/file (il faut bien verifier le plus long prefixe)
