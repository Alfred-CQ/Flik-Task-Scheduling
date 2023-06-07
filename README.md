# Flick-Task-Scheduling

Implementation of a Hybrid Ant Genetic Algorithm for Efficient Task Scheduling in Cloud Data Centres

## **Makespan as the main scheduling objective**

The makespan $\mathbf{MT}$ represents the length of the time interval from the start of the first task to the completion of the last task in a set of scheduled tasks. The goal is: minimise the makespan in order to achieve optimal efficiency and utilisation of available resources.

Evolutionary algorithms iterate over all possible solutions to get an optimal solution. Their convergence time $\mathbf{CT}$ depends on solution space and the number of iterations.

Therefore, the objective function $O$ to find solution $x$ is as follows:

$\mathbf{O(x)} = \mathbf{min}(MT), \mathbf{min}(CT)$

### **Virtual Machines (VM's) and Tasks**

Suppose there are $m$ virtual machines and $n$ tasks in the system then set of virtual machines $\mathbf{V}$ and tasks $\mathbf{T} $ can be represented as:

$\mathbf{V} = \{v_{1}, v_{2}, v_{3}, \ldots, v_{m}\}$

$\mathbf{T} = \{t_{1}, t_{2}, t_{3}, \ldots, t_{n}\}$

### **Allocation of Tasks**

The tasks can be allocated to VM's in $n^m$ ways.
$\mathbf{R}$ is a set of virtual machine resources and can be represented as:

$\mathbf{R} = \{r_{1}, r_{2}, r_{3}, \ldots, r_{k}\}$

These resources include:

| Virtual Machine Resources | Description                                                                             |
| ------------------------- | --------------------------------------------------------------------------------------- |
| CPU Capacity              | Ability to execute millions of instructions per second (MIPS)                           |
| Bandwidth                 | Data transfer speed                                                                     |
| RAM                       | Memory used for temporary data storage                                                  |
| Storage                   | Storage space for saving files and data                                                 |
| **Compute Unit (CU)**     | Measurement units used by cloud service providers to calculate virtual machine capacity |

### **Execution Time and Estimated Running of the Tasks**

The execution time of the tasks and the estimated running cost of the workflow can be calculated by $CU$

Therefore, execution time $\mathbf{ET}$ of task $t$ can be expressed as:

$ET_{ij} = \frac{L_{i}}{CU_{j}}$

Where, $i ∈ {1, 2, 3, ...., n}, j ∈ {1, 2, 3, ....m}$, $\mathbf{L_{i}}$ is the length of the task $t_i$ and $\mathbf{CU_{j}}$ represents $\mathbf{CU}$ of a virtual machine $v_j$.

Busy time $\mathbf{BT}$ of $vm_j$ can be expressed as:

$BT_{j} = \sum_{i=1}^{N} ET_{ij} \cdot c_{ij}$

Where, ${c_{ij}} ∈ {0, 1}$ and ${c_{ij}}$ represents the relationship between virtual machine and task. If ${c_{ij}}$ is 1 then the task $t_i$ is scheduled on a
virtual machine $v_j$.

### **Makespan(MT)**

As VM's run in parallel, the running time of workflow is equal to the maximum busy time of a VM
We can calculate the $\mathbf{MT}$ of the system by:

$\mathbf{MT} = max(BT_{j})$

## Scheme of the datasets

The index below is aligned with the data column in each file.

### VMachine_Flik

The Virtual Machine Flik Dataset consists of information about 30 virtual machines

| Field              | DataType | Description                                                  |
| ------------------ | -------- | ------------------------------------------------------------ |
| Virtual Machine Id | String   | Unique identifier of the virtual machine.                    |
| Processor Name     | String   | Name or model of the processor in the machine.               |
| Processor MIPS     | Integer  | MIPS (Millions of Instructions Per Second) of the processor. |
| Clock Frequency    | Float    | Clock frequency of the processor in GHz.                     |
| Bandwidth          | Integer  | Bandwidth in Mbps.                                           |
| RAM                | Integer  | Amount of RAM in GB.                                         |
| Storage            | Integer  | Storage capacity in GB.                                      |
| CU (Compute Unit)  | Float    | Compute Unit calculated as (MIPS \* Clock Frequency) / 1000. |

### Task_Flik

The Task Flik Dataset contains information about 100 tasks.

| Field     | DataType | Description                                      |
| --------- | -------- | ------------------------------------------------ |
| Task_Id   | String   | Unique identifier for each task                  |
| MIPS      | Integer  | Processing power of the task (in MIPS)           |
| RAM       | Integer  | Amount of memory required by the task (in GB)    |
| Bandwidth | Integer  | Network bandwidth required by the task (in Mbps) |
| Duration  | Float    | Estimated duration of the task (in minutes)      |
