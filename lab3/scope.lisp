(let ((x 5))   ;; lexical variable

(defun foo_static ()
    (format t "foo sees x = ~a~%" x))

(defun bar_static ()
    (let ((x 10))
    (foo_static)))

(bar_static))

;; Expected: 5


(defparameter *x* 5)  ;; dynamic variable

(defun foo_dynamic ()
  (format t "foo sees x = ~a~%" *x*))

(defun bar_dynamic ()
  (let ((*x* 10))   ;; dynamic binding
    (foo_dynamic)))

(bar_dynamic)

;; Expected: 10