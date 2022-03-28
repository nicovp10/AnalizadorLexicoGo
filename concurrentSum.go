package main

import "fmt"

func sum(s []float32, c_actual chan float32) {
        var sum float32 = 0
        for _, v := range s {
                sum += v
        }
        c_actual <- sum // send sum to c_actual
}

func main() {
        /**
         * /This program concurrently calculates the total sum of a float array,
         * by using two goroutines writing partial results to a shared channel.
         */
        s := []float32{2, real(1.e0+0i), real(1.e+0i), 0xBadFace42*1e-11, -9,4.e+0,.1273E2}

        c_actual := make(chan float32)
        go sum(s[:len(s)/2], c_actual)
        go sum(s[len(s)/2:], c_actual)
        i, j := <-c_actual, <-c_actual // receive from c_actual

        fmt.Println("Partial results and \"Total\" sum:");
        fmt.Println(i, j, i+j)
}