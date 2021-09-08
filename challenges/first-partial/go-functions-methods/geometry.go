// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.y, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

func onSegment(p, q, r Point) bool {
	if q.X() <= math.Max(p.X(), r.X()) && q.X() >= math.Min(p.X(), r.X()) && q.Y() <= math.Max(p.Y(), r.Y()) && q.Y() >= math.Min(p.Y(), r.Y()) {
		return true
	}
	return false
}

func orientation(p, q, r Point) int64 {
	val := (q.Y()-p.Y())*(r.X()-q.X()) - (q.X()-p.X())*(r.Y()-q.Y())
	if val == 0 {
		return 0 //Colinear
	}
	if val > 0 {
		return 1 // Clock
	} else {
		return 2 // Counterclock wise
	}
}

func doIntersect(p1, q1, p2, q2 Point) bool {

	// Casos especiales
	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)

	// Caso general
	if o1 != o2 && o3 != o4 {
		return true
	}

	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if o1 == 0 && onSegment(p1, p2, q1) {
		return true
	}
	// p1, q1 and q2 are colinear and q2 lies on segment p1q1

	if o2 == 0 && onSegment(p1, q2, q1) {
		return true
	}

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if o3 == 0 && onSegment(p2, p1, q2) {
		return true
	}

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if o4 == 0 && onSegment(p2, q1, q2) {
		return true
	}

	return false
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	s := ""
	for i := range path {
		fmt.Printf("  - (%.2f,%.2f)", path[i].X(), path[i].Y())
		fmt.Println()
		if i > 0 {
			dist := path[i-1].Distance(path[i])
			sum += dist
			s += fmt.Sprintf("%.2f + ", dist)
		}
	}
	ultimo := path[0].Distance(path[len(path)-1])
	sum += ultimo
	s += fmt.Sprintf("%.2f", ultimo)
	fmt.Printf("- Figure's Perimeter\n")
	fmt.Printf("  - %s", s)
	fmt.Printf(" = %.2f", sum)
	fmt.Println()
	return sum
}

func main() {

	if len(os.Args) < 2 {
		fmt.Printf("Please, write at least one argument\n")
		fmt.Println("Example: ")
		fmt.Println("# go run geometry.go <number_of_sides>")
		return
	}
	sides, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println("You must write an Integer")
		fmt.Println("Example: ")
		fmt.Println("# go run geometry.go <number_of_sides>")
		return
	}
	if sides <= 2 {
		fmt.Printf("Please, provide a number greater or equal than three\n")
		return
	}

	path := createIrregularFigure(sides)
	path.Distance()
}

func createIrregularFigure(points int) Path {
	// Creamos array de points
	path := make(Path, points)
	sides := len(path)

	fmt.Printf("- Generating a [%d] sides figure\n", sides)
	fmt.Printf("- Figure's vertices")
	fmt.Println()

	if sides > 3 { // Sí el polígono es mayor a 3 puntos
		for i := 0; i < sides; i++ {
			path[i] = getPoint() // Obtenemos un punto con valores aleatorios
			if i >= 4 {          // Cuando tenemos al menos 2 líneas
				p1 := path[i-3]                          // Eje: X - Linea 1
				q1 := path[i-2]                          // Eje: Y - Linea 1
				p2 := path[i-1]                          // Eje: X - Linea 2
				q2 := path[i]                            // Eje: Y - Linea 2
				intersect := doIntersect(p1, q1, p2, q2) // Sí ambas líneas se interceptan entonces es true.
				for intersect {                          // Sí la línea sigue interceptando, entonces seguimos calculando con un punto distinto cada vez.
					p2 = getPoint()
					path[i-1] = p2
					intersect = doIntersect(p1, q1, p2, q2)
				}
			}
		}
	} else {
		for i := 0; i < sides; i++ {
			path[i] = getPoint() // Sí el polígono tiene 3 vertices
		}
	}

	return path
}

func generateRandom(min float64, max float64) float64 {
	rand.Seed(time.Now().UnixNano())
	return min + rand.Float64()*(max-min)
}

func getPoint() Point {
	x, y := generateRandom(-100, 100), generateRandom(-100, 100)
	return Point{x, y}
}
