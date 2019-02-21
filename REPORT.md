# Report monitoring the usage of cpu and memory

>  for a better view, read this report on the following link: [github](https://github.com/gideaopinheiro/AB2.1-/blob/master/REPORT.md)

## Identification

Student 1
- Name: Gideão Pinheiro dos Santos Silva
- Registration: 18211778
- Score Percentage: 75%

Student 2
- Name: Gabriel Araujo Ferreira
- Registration: 18211744
- Score Percentage: 25%


## Results
#### Usage graphs

> The graphs used in this report were obtained from the rstudio software, 
> read the [INSTALL.md](https://github.com/gideaopinheiro/AB2.1-/blob/master/INSTALL.md) for more informations

When performing the command:
```sh
$ ./bin ucp
```

The program performs the function responsible for forcing the ucp
and a graph similar to this will be obtained from the data collected

![justucp](https://github.com/gideaopinheiro/AB2.1-/blob/master/static/justucp.png)


When performing the command:
```sh
$ ./bin ucp-mem
```
The program performs the function responsible for forcing the ucp and memory.
This is the graph of ucp usage

![ucp](https://github.com/gideaopinheiro/AB2.1-/blob/master/static/ucp.png)

And this is the graph of memory usage

![mem](https://github.com/gideaopinheiro/AB2.1-/blob/master/static/mem.png)

----

## Discussion
#### About the first graph
  Graph 1 shows an expected CPU behavior during program performing. Even the machine processor that was used for testing having 2 GHz capacity, which means 2 billion instructions in 1 second, the graph shows that the CPU usage was always close 
to 100%. It is important to remember that the cpu of the computer used counts on 2 cores and therefore some values appear in the graphic with value greater than 100%, which is the sum of the percentage of the two cores.
	
  The code written in the child process for when the "ucp" argument is passed to the main function, it perform an infinite loop, that is, the condition never becomes false and therefore it would perform without interruptions, infinitely. In this case, it is necessary to force the interruption of the program.
	
  Because the condition within the while loop in the child process (line 105) is always true, the stop condition is made inside the parent process, which monitors the behavior of the child and allows it to run for only 10 seconds. After 10 seconds, the parent process performs a system call (line 51), called kill that serves to kill a process, identified by a pid (process identification).
  
![codigo_2](https://github.com/gideaopinheiro/AB2.1-/blob/master/static/codigo_2.png)
![codigo_1](https://github.com/gideaopinheiro/AB2.1-/blob/master/static/codigo_1.png)

  If in the first second after killing the process, we reapplied CPU consumption of the process, we would see a curve as sharp as the first second, but going from some high value to zero, since a dead process does not use cpu.
  
 
  #### About the other two graphs

  Graphs 2 and 3 show the behavior of cpu and memory, respectively.
  Graph 2 is similar to graph 1, because an infinite loop is also perform when the "ucp-mem" argument is passed to the main function. But even though they are similar, the consumption shown in the first one is much higher than in the second, because in the second graph the program is performing an infinite loop and allocating an amount of memory, but in order to do so it has to be stopped repeatedly, decreasing CPU consumption.

  If this allocation were made without control, the program, due to the processor's capacity, would have the computer reserve millions of MegaBytes, interrupting the running of other processes and preventing the computer from working. Therefore, before each allocation, the program waits 1 millisecond, causing the amount of memory allocated per second to be sufficiently low so that the other processes can continue running.
 
  The **usleep()** function (line 109) causes a large decrease in cpu consumption, around 90% because the infinite loop is interrupted every millisecond for memory allocation and then resumed. This shows how different the time dimension is from the point of view of the machine and the man.

![codigo_2](https://github.com/gideaopinheiro/AB2.1-/blob/master/static/codigo_2.png)

  The third graph shows the memory allocation of the child process. Continuous growth occurs because the memory allocated during program execution remains allocated until the process no longer exists. If we wanted to free up space while the program was running, we could have used the [free()](http://www.cplusplus.com/reference/cstdlib/free/?kw=free) function that frees up the allocated memory space. But that was not the case.


#### Obtaining the graphs with RStudio

> For this report, we used RStudio 1.1.46
> and the ggplot2 package

To load the data we use the **library()** function that loads an already installed library
```sh
library(readr)
```
We assign to the variable **arquivo_tesste** the file **arquivo_test.csv**
and for RStudio to find the file, we give it the directory where the file is located.
If you want to perform this script in your machine, change the directory.
```
arquivo_teste <- read_csv("Documentos/ComputerScience/Computer/arquivo_teste.csv")
```
If you don't have the ggplot2 installed, perform the followed script. 
```
install.packages("ggplot2")
require(ggplot2)
```
In this case, we need create a vector with 10 values from 1 to 10.
Let's call it seconds.
```
seconds <- c(1,2,3,4,5,6,7,8,9,10)
```
Now, let's to perform the script that create the first graphic.
In this function we need to pass the file as the first parameter.
then the fields corresponding to the Cartesian axes x and y.
at the end, the type of graphic, that in our case is the line graphic.
```
ggplot(arquivo_teste, aes(x=seconds, y=ucp_usage)) + geom_line()
```

Before plotting the other graphics, the file name must be changed within the source code for **arquivo_teste2.csv**, on the **65** line.
![codigo_3](https://github.com/gideaopinheiro/AB2.1-/blob/master/static/codigo_3.png)

then it must be compiled with the ```$ gcc main.c -o bin``` command and executed again with the ```$ ./bin ucp-mem``` command (on bash).

After that, run the following script to read the file and plot the other two graphs. 
```
arquivo_teste2 <- read_csv("Documentos/ComputerScience/Computer/arquivo_teste2.csv")
```
```
ggplot(arquivo_teste2, aes(x=seconds, y=ucp_usage)) +
  geom_line()
```
```
ggplot(arquivo_teste2, aes(x=seconds, y=mem_usage)) +
  geom_line()
```
