package main

import "fmt"

func sum(s []float32, c chan float32) {
        var sum float32 = 0
        for _, v := range s {
                sum += v
        }
        c <- sum // send sum to c
}

func main() {
        /**
         * /This program concurrently calculates the total sum of a float array,
         * by using two goroutines writing partial results to a shared channel.
         */
        s := []float32{2, real(1.e0+0i), real(1.e+0i), 0xBadFace42*1e-11, -9,4.e+0,.1273E2}

        c := make(chan float32)
        go sum(s[:len(s)/2], c)
        go sum(s[len(s)/2:], c)
        i, j := <-c, <-c // receive from c

        fmt.Println("Partial results and \"Total\" sum:");
        fmt.Println(i, j, i+j)
}