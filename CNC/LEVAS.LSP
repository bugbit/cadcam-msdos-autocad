(defun sqr(num) (* num num))

(defun CALCY(num) (* M num))

(defun INTERARCO(angp angf / CENTRO A B C RAIZ X3 Y3 X4 Y4 angx3 xc yc)

   (setq xc (cadr (setq CENTRO (assoc 10 alist))))
   (setq yc (caddr CENTRO))
   (setq A (+ (sqr M) 1))
   (setq B (* -2 (+ xc (* m yc))))
   (setq C (+ (sqr xc) (sqr yc) (* -2 yc m) (- (sqr (cadr (assoc 40 alist)
   )))))
   (setq RAIZ (+ (sqr B) (* -4 A C)))
   (if (>= RAIZ 0)
      (progn
         (setq SA 1)
         (setq Y3 (CALCY (setq X3 (+ (- B) (sqrt RAIZ)))))
         (setq Y4 (CALCY (setq X4 (- B (sqrt RAIZ)))))
         (setq angx3 (/ (* (atan (/ (+ Y3 (- yc)) (+ X3 (- xc)))) 180) PI))
         (if (< yc y3)
            (if (> xc x3) (setq angx3 (+ 180 (- angx3))))
            (if (> xc x3)
               (setq angx3 (+ 180 angx3))
               (setq angx3 (+360 (- angx3)))
               )
            )
         (if (and (> (cadr angp) angx3) (< (caddr angf) angx3))
            (progn
               (setq X2 X3)
               (setq Y2 Y3)
               )
            (progn
               (setq X2 X4)
               (setq Y2 Y4)
               )
            )
         )
      )
   )

(defun CPUNTO(alist / SA)

   (setq SA 0)
   (if (= (setq FF (assoc 0 alist)) "CIRCLE")
      (INTERARCO (cons 50 0) (cons 51 360))
      (if (= FF "ARC")
         (INTERARCO (assoc 50 alist) (assoc 51 alist))
         )
      (if (= FF "LINE")
         (progn
            (setq li (INTER X1 Y1 (cadr (setq li (assoc 10 alist)))
            (caddr li) (cadr (setq li (assoc 11 alist))) (caddr li)))
            (if (/= li nil)
               (progn
                  (setq X2 (car li))
                  (setq Y2 (cadr li))
                  (setq SA 1)
                  )
               )
            )
         )
      )
   )

(defun DCARRO(/ CNOMB X1 Y1 FI M SL2 DX)
   
   (setq CNOMB (strcat "CARRO" NSTR))
   (if (/= (setq FI (entget (ssname (ssget "X" (list (cons 0 "CIRCLE")
   (cons 8 CNOMB))) 0))) nil)
      (progn
         (setq X (cadr (setq CE (assoc 10 FI))))
         (setq Y (caddr CE))
         (if (/= (setq FI (entget (ssname (setq SL2 (ssget "X" (list (cons 0
         "LINE") (cons 8 CNOMB)))) 0))) nil)
            (progn
               (setq X1 (cadr (setq CE (assoc 10 FI))))
               (setq Y1 (caddr CE))
               (setq NL (sslength (setq SL (ssget "X" (list (cons 8 (strcat
               "LEVA" NSTR)))))))
               (setq N -1)
               (setq M (if (= (setq DX (- X1 X)) 0) (eval 1) (/ (- Y1 Y) DX)))
               (while (< (setq N (1+ N)) NC)
                  (CPUNTO (entget (ssname SL N)))
                  )
               (command "DESPLAZA" SL2 "" CE C2)
               )
            )
         )
      )
   )

(defun CNARCO(/ CE)

  (setq X (+ X (cadr (setq CE (assoc 10 ENTI)))))
  (setq Y (+ Y (caddr CE)))

  )

(defun CNLINEA(/ CE CE2)

  (setq X (+ X (/ (+ (cadr (setq CE (assoc 10 ENTI))) (cadr (setq CE2 (assoc
  11 ENTI)))) 2)))
  (setq Y (+ Y (/ (+ (caddr CE) (caddr CE2)) 2)))

  )

(defun GLEVA(/ SL NL N X Y ENTI NOMBRE)

  (setq NL (sslength (setq SL (ssget "X" (list (cons 8 (strcat "LEVA" NSTR)))
  ))))   
  (setq N -1 X 0.0 Y 0.0)
  (while (< (setq N (1+ N)) NL)
     (progn
       (setq ENTI (entget (ssname SL N)))
       (if (or (= (setq NOMBRE (cdr (assoc 0 ENTI))) "CIRCLE") (= NOMBRE
       "ARC"))
         (CNARCO) (CNLINEA)
         )
       )
     )
  (command "GIRA" SL "" (list (/ X NL) (/ Y NL)) ang)

  )

(defun LEVA(numl / NSTR)
   
   (prompt (strcat "LEVA N. " (setq NSTR (itoa numl))))
   (GLEVA)
   (prompt "\n")
   )

(defun PNLEVA(nl)

  (setq les (append les (list nl)))

  )

(defun TDLEVAS(/ cap str2 alist)

  (setq cap (tblnext "LAYER" T))
  (while (/= cap nil)
     (progn
       (if (and (/= (setq alist (assoc 2 cap)) nil) (= (substr (setq str2 (cdr
       alist)) 1 4) "LEVA")) (PNLEVA (atoi (substr str2 5))))
       (setq cap (tblnext "LAYER"))
       )
     )
  )

(defun ESLEVA(nl / cap)

  (setq cap (strcat "LEVA" (itoa nl)))
  (if (/= (tblsearch "LAYER" cap) nil) (PNLEVA nl))
  )

(defun CGLEVA(/ num str2)

  (if (<= n1 n)
     (progn
       (setq str2 (substr str n1 (- n n1 -1)))
       (setq num (atoi str2))
       (ESLEVA num)
       (setq n1 (1+ n))
       )
     )
  )

(defun MILEVAS(str / len n n1)

  (setq len (strlen str))
  (setq n1 (setq n 1))
  (while (<= n len)
     (progn
        (if (= (substr str n 1) ",") (CGLEVA))
        (setq n (1+ n))
        )
     )
  (CGLEVA)
  )

(defun CGLEVAS(str / len n n1)

  (setq les nil)
  (if (= str "") (TDLEVAS) (MILEVAS str))
  )      

(defun C:LEVAS(/ ocmd les l)

   (setq ocmd (getvar "CMDECHO"))
   (setvar "CMDECHO" 0)
   (setq ang (getint "Introduzca el angulo de rotacion de las levas : "))
   (CGLEVAS (setq le (getstring "Introduzca las levas a mover <*> : ")))
   (foreach l les (LEVA l))
   (setq ang nil)
   (setvar "CMDECHO" ocmd)
   (eval nil)
   )
