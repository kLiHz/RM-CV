# Principal Components Analysis (PCA)

https://docs.opencv.org/3.4/d1/dee/tutorial_introduction_to_pca.html

Principal Components Analysis is a statistical procedure that extracts the most important feature of a dataset. A key point of PCA is the Dimensionality Reduction. Dimensionality Reduction is the process of reducing the number of the dimensions of the given dataset. PCA allows us to find the direction along which our data varies the most. Briefly speaking, using PCA allows us to find some of the most important components in a dataset, i.e., some of the most important variables when describing a certain thing.

 

Imagine that we describe a thing with N values, with each one called a component. These values will form a N-dimensional vector. Applying PCA to a N-dimensional dataset yields N N-dimensional eigenvectors, N eigenvalues and a N-dimensional center point. Those eigenvectors are the *principal components* of the dataset. The size of each eigenvector is encoded in the corresponding eigenvalue, indicating how much the data vary along the principal component.

 

**Organize the dataset**

Suppose we have a set of data, each of them has $p$ variables, and we want to reduce the data so that each one can be described with only $L$ variables, $L<p$. Each entry can be represented as a $n$-dimensional row vectors $x_1, x_2, ..., x_n$. And by putting these vectors together we can get a matrix $\bf{X}$ of dimensions $n\times p$.

**Calculate the empirical mean**

Next, we are supposed to find the empirical mean along each dimension $j$, $j=1,2,...p$. Then we put the mean values into a vector $\bf{u}$ of dimensions $p\times 1$.
$$
{\bold u[j]} = \frac{1}{n}\sum_{i=1}^{n}{\bold X[i,j]}
$$
**Calculate the deviations from the mean**

To find a principal component basis that minimizes the mean square error of approximating the data, we subtract the empirical mean vector $\bf u$ from each row of the data matrix $\bf X$, and store the mean-subtracted data in the $n\times p$ matrix $\bf B$.
$$
\mathbf{B} = \mathbf{X} - \mathbf{h}\mathbf{u^{T}}
$$


where $\bf h$ is an $n \times 1$ column vector of all 1s: 
$$
\bold h[i] = 1, i = 1, ..., n
$$
**Find the covariance matrix**

The $p \times p$ empirical covariance matric $\bf C$ is the outer product of matrix $\bf B$ with itself:
$$
\mathbf{C} = \frac{1}{n-1} \mathbf{B^{T}} \cdot \mathbf{B}
$$
**Find the eigenvectors and eigenvalues of the covariance matrix**

The matrix $\bf V$ of eigenvectors diagonalizes the covariance matrix $\bf C$:
$$
\mathbf{V^{-1}} \mathbf{C} \mathbf{V} = \mathbf{D}
$$
where $\bf D$ is the diagonal matrix of eigenvalues of $\bf C$.

Matrix $\bf D$ is a $p \times p$ diagonal matrix:
$$
\bold D[k,l] = \left\{\begin{matrix} \lambda_k, &k = l \\ 0, &k \neq l \end{matrix}\right.
$$
where ${\lambda}_j$is the $j$-th eigenvalue of the covariance matrix $\bf C$.

Matrix $\bf V$, with its dimension of  $p \times p$,  represent the  $p$eigenvectors of the covariance matrix $\bf C$.



