(def x '(1 2 3 4 5))
(display "x: ")
(display x)
(newline)
(def y (make-tconc))      
(tconc! y 1)
(display "y: ")
(display y)
(newline)
(tconc-splice! y x)
(display "y: ")
(display y)
(newline)
(def z (make-tconc))
(tconc! z 1)
(tconc! z 3)
(display "z: ")
(display z)
(newline)
(tconc-splice! y z)
(display "y: ")
(display y)
(newline)