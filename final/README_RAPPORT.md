PIERRE GARNIER






## DOMAINE 1: carré [0,1]x[0,1]

* -delta u = fomega
    1. u(x,y) = 16xy(1-x)(1-y) -> fomega = 32(x(1-x)+y(1-y))

    2. u(x,y) = sin(PIx)sin(PIy) -> fomega = 2PI^2*sin(PIx)*sin(PIy)

* -delta u + u = fomega
    1. u(x,y) = cos(PIx)cos(PIy) -> fomega = (1+PI^2)*cos(PIx)*cos(PIy)

## DOMAINE 2: carré [0,1/2]x[0,1/2]

## Résultats:

On s'attarde principalement sur le premier exemple.

Pour un maillage en triangles on a:

| Intervalles de subdivision | Erreur quadratique relative | Erreur maximum relative|
|----------------------------|-----------------------------|------------------------|
| 2                          | 0.166666627                 | 0.166666627            |
| 4                          | 0.478788316                 | 0.503116250            |
| 8                          | 

Pour un maillage en quadrangles on a:

| Intervalles de subdivision | Erreur quadratique relative | Erreur maximum relative|
|----------------------------|-----------------------------|------------------------|
| 2                          | 0.249999881                 | 0.249999881            |
| 4                          |                             |                        |
| 8                          | 

Une interprétation des données est presque ridicule, on aurait tendance à dire que l'erreur augmente avec le nombre de subdivisions
mais avec deux points mais avec seulement deux points c'est difficile à dire.
Cependant cela peut être expliqué par le fait que je nombre d'opérations augmente avec le nombre de subdivisions et donc les erreurs
s'accumulent. On doit pouvoir compenser cette erreur qui augmente en faisant augmenter l'ordre du maillage.



    
 