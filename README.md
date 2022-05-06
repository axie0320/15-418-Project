# **15-418 Project**
15-418 Final Research Project on Parallel Matrix Inversion 
<br><br>

## **Title** 
---
### **Parallel Matrix Inversion**
by Allen Xie and Chi Nguyen
<br><br>

## **Summary**
---
We are going to create and optimize parallel implementations of matrix inversion. We plan on parallelizing key matrix decomposition/elimination algorithms that will ultimately allow us to parallelize matrix inversion. 
<br><br>

## **Background**
---
Matrix Inversion is very useful in solving various real-world problems, especially in computer graphics, such as 3D graphics rendering and real-time 3D simulations. There are mumerous matrix decomposition and elimination algorithms that can be utilized for finding matrix inverses. Among the existing methods of matrix inversion, we’ll focus on parallelizing LU Decomposition and Gauss-Jordan Elimination for this particular project. 

For Gauss-Jordan Elimination, the general idea is to create an augmented system of the form (A || B) where A is the n x n matrix we want to invert and B is the n x n identity matrix. Then, we perform Gaussian elimination on this system to convert the left matrix into the identity matrix and what the right matrix becomes after this step is the inverted input matrix.  

For LU Decomposition, we want to factorize our n x n matrix A into two factors L and U, where L is a lower triangular matrix and U is an upper triangular matrix. Then, we perform elimination on L and U to find their inverse matrices. The inverted A is the product of inverted L and inverted U. 

We notice that there are various interesting opportunities for parallelization in the process of matrix elimination and decomposition. First and most obvious are the low level matrix operations. We can identify independence among matrix rows, columns or cells to speed up overall computation. There are also less obvious ways of breaking down the original problem into smaller matrices and recombining them. Taking the example of LU Decomposition, certain algorithmic approaches allow us to calculate the lower and upper triangular matrices independently in parallel, while other approaches allow us to operate on smaller matrices in parallel and then recombine back to lower and upper triangular matrices.
<br><br>

## **The Challenge**
---
Matrix decompositions and Gaussian elimination are central operations in calculating matrix inverses. Algorithms for achieving such tasks are often highly sequential and simple parallelization might only be obvious on low level operations within them. A major challenge with further parallelizing these algorithms is identifying dependencies among operations on matrix rows and columns. In addition, recursively breaking down the matrices into smaller problem sizes does not allow easy combinations back to solve the original problem. The subproblems and process of recombining to solve the original problem also raises the challenges with distributing workload.
<br><br>

## **Resources**
---
Research articles: We will be referencing and studying existing research on large scale matrix inversion to identify areas of parallelization

  * <a href="https://www.cs.utexas.edu/~flame/pubs/SIAMMatrixInversion.pdf">A Note On Parallel Matrix Inversion</a>
  * <a href="https://www.mdpi.com/2078-2489/11/11/523">A Method of Ultra-Large-Scale Matrix Inversion Using Block Recursion</a>

We will be writing code from scratch, only referencing cited research for checking theoretical understanding and correctness 
<br><br>

## **Goals and Deliverables**
---
The goal of this project is to parallelize various implementaitons of inversing a matrix and analyze their performance trade-offs. There are two approaches of interest we will specifically focus on: matrix inversion through LU Decomposition and Gauss-Jordan Elimination. 

* PLAN TO ACHIEVE: 
  * We plan to finish implementing both parallel and sequential matrix inversions using LU Decomposition and Gauss-Jordan Elimination.
  * We wish to provide performance analysis on the two different approaches and charts that illustrate performance improvements across various different workload levels and thread counts.

* HOPE TO ACHIEVE:
  * If we are ahead of schedule and complete all tasks under ‘plan to achieve’, we wish to write our own parallel implementation that can hopefully integrate the strengths of both approaches.

* BACKUP PLANS: 
  * In case we face significant obstacles and delays on our schedule, we wish to at least complete the block parallel implementation of LU Decomposition and Gauss-Jordan Elimination. Due to the constraints, we might theorize their potential performance impacts on parallel matrix inversion. We will support the theories by testing these two implementations directly instead of implementing and testing on matrix inversion. 
<br><br>

## **Platform**
---
We plan to use GHC Cluters, C++ and OpenMP for this project. Becauese potential parallel implementations might require recursion, we wish to leverage and optimize with openMP's task scheduling features. 
<br><br>

## **Schedule**
---
Proposed schedule:
<br><img src=".\docs\images\schedule_draft.jpg" width='500px'></img>  
<br><br>

## **Milestone Report**
---

<p>We were able to complete the initial sequential and parallel implementation of matrix inverse with Gauss-Jordan method. For the initial Gauss-Jordan implementations, we were able to parallelize most of the computation, achieving a work of O(n^3) and span of O(n). However, we noticed that most of the speedup comes from parallelizing low-level matrix operations/computations, which limits the efficiencies when inversing extremely large matrices. We believe such problems can be addressed using a block recursive method. This aligns with the research direction from our original plan and is something we are currently working on. As for the LU decomposition method, we have been able to complete the initial sequential implementation of this method using blocks. The next step is to parallelize the computuation using openMP. </p>

<p>With respect to the goals and deliverables from the proposal, we are on track for most of our goals. The only area we are slightly behind on implementing is writing robust testing code. We plan on prioritizing this area in the upcoming week as we move closer to performance analysis in the later weeks.</p>

<p>Our current plan for the poster session will be a collection of statistical graphs alongside detailed analysis. From our work at the moment, we believe the info will focus mostly on comparing parallelization implementations within each algorithmic approach as well as contrasting the two approaches. </p>

<p>The main concern we face at the moment is primarily coding. There were some challenges with understanding existing research around the topic of our interest, but fortunately, as we started working on our parallelizations, we understood a lot of the research better. </p>

<br><img src=".\docs\images\schedule_detailed.jpg" width='500px'></img>  
<br><br>

## **Final Presentation**

Link to our final presentation: https://drive.google.com/file/d/1h5YNGatdl_svlZcG2Q0f8wFho5MBOjEW/view?usp=sharing

<br><br>
---
