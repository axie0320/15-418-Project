# **15-418 Project**
15-418 Final Research Project on Parallel Matrix Inversion 
<br><br>

## **Title** 
---
### **Parallel Matrix Inversion**
by Allen Xie and Chi Nguyen
<br><br>

## **Summary**
We are going to create and optimize parallel implementations of matrix inversion. We plan on parallelizing key matrix decomposition/eliminations algorithms that will ultimately allow us to parallelize matrix inversion. 

<br><br>
---
The goal of this project is to parallelize various implementaitons of inversing a matrix and analyze their performance trade-offs. There are two approaches of interest we will specifically focus on: matrix inversion through LU Decomposition and Gauss-Jordan Elimination. 
<br><br>

## **Background**
---
Matrix Inversion is very useful in solving real-world problems, especially in computer graphics, such as 3D graphics rendering and real-time 3D simulations. There are various matrix decomposition and elimination algorithms that can be utilized for finding matrix inverses. Among the existing methods of matrix inversion, we’ll focus on parallelizing LU Decomposition and Gauss-Jordan Elimination for this particular project. 

For Gauss-Jordan Elimination, the general idea is to create an augmented system of the form (A || B) where A is the n x n matrix we want to invert and B is the n x n identity matrix. Then, we perform Gaussian elimination on this system to convert the left matrix into the identity matrix and what the right matrix becomes after this step is the inverted input matrix.  

For LU Decomposition, we want to factorize our n x n matrix A into two factors L and U, where L is a lower triangular matrix and U is an upper triangular matrix. Then, we perform Gaussian elimination on L and U to find their inverse matrices. The inverted A is the product of inverted L and inverted U. 


<br><br>

## **The Challenge**
---


<br><br>

## **Resources**
---
* Research articles 

<br><br>

## **Goals and Deliverables**
---


## **Platform**
---
We plan to use GHC Cluters, C++ and OpenMP for this project 
<br><br>

## **Schedule**
---
Proposed schedule:
<br><img src=".\docs\images\schedule_draft.jpg" width='500px'></img>  
