# Progression TP

## TP1 - Maillage + num de référence

Conf file :

    0 1 0 1      -> a b c d (float)
    n1 n2        -> nb of points per side (int>0)
    t            -> type of element (int=1 | int=2)
    0 1 2 3 4    -> reference numbers : domain, side1, side2, side3, side4 (int)

    Same layout in file

Mesh file :

    one element : 1 2 5 1 0 4
        first three : index of tops
        last three : reference numbers of edges

    file structure :
        n                     -> nb of nodes
        x1 y1                 -> coordinates of nodes  
        ...
        xn yn
        m t p q               -> m : nb of els | p : nb of nodes per el | t : type of el | q : nb of edges per el
        s11 .. si1 r11 .. ri1
        ...
        s1m .. sim r1m .. rim

    t = 1 : 
        shape of node : quadrangle
        p = 4
        q = 4

    t = 2 :
        shape of node : triangle
        p = 3
        q = 3

    Arrays :
        coord [nx2] float
        ngnel [mxp] int
        nRefAr [mxq] int

## TP2 - Calculs élémentaires

## TP3 - Assemblage

## TP4 - Système linéaire

## TP5 - Résolution + exploitation