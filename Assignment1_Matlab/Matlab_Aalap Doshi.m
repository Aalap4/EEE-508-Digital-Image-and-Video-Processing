%We perform the Matlab code for Image Processing and perform 1D and 2D convolution 
close all
a=imread('cameraman.jpg'); %reads the image cameraman.jpg
ax1=subplot(2,2,1);
imshow(a)                   %Display the Original image
title(' Original Image')

h= [1/9 1/9 1/9; 1/9 1/9 1/9; 1/9 1/9 1/9]  %Creates the 3X3 2D impluse response matrix 

y1= conv2(a,h);            % Performs filtering using conv2() function                       
ax2= subplot(2,2,2);
imshow(uint8(y1))
title('Y1')                % Filtered image Y1

y2= filter2(h,a);          % Performs filtering using filter2() function  
ax3= subplot(2,2,3);
imshow(uint8(y2))
title('Y2')                %Filtered image Y2

ax4=subplot(2,2,4);
compare_1 = imshowpair(y1,y2);
title('Compare Y1 AND Y2')    % Compared image output

S= isfilterseparable(h) % if the return is logic 1 the matrix h is separable

%Our impluse response h(n1,n2) is separable.
%Separability can also be found by finding the rank of the matrix and if the rank is 1 then it is separable.
%Separable matrix means the given matrix can be derived from convolution
%of two 1D vectors either row and column.

[U S V]= svd(h) %we perform svd to find out row and column whoes multiplication can result in matrix h 
h1 = U*sqrt(S)                          % we take squareroot of S
h2 = sqrt(S)*V'
h1 = U(:,1)*sqrt(S(1,1))                %column 
h2 = sqrt(S(1,1))*V(:,1)'               %row

% we multiply h1*h2 to check whether we get the h matrix again, so (separable_a = h)
separable_a= h1*h2                      

for i= 1:256                              %Dconv(i,:) are the rows of the matrix
    Dconv(i,:)= conv(h2,a(i,:));          %conv the rows with the image a
end
for i = 1:256                             %DDconv(:,i) are the columns of the matrix
    DDconv2(:,i) = conv(h1, Dconv(:,i));  %conv the column with the each column of Dconv(i,:)  
end

y3= uint8(DDconv2);                       % Image after 1D convolution
figure(2)
imshow(y3)
title('Y3');                              

figure(3)
compare_2 = imshowpair(y1,y3);
title('Compare Y1 AND Y3')                % Compared image output

%Our filter is a low pass filter. We will convert it to high pass filter
n=[1,0,0;0,0,0;0,0,0];  %Defining delta function
h2=n-h;                 % This forms the highpass filter
 
n_highpass=filter2(h2,a);
figure(4),
imshow(uint8(n_highpass))                 %Displaying the highpass filtered image
title('Image after highpass filtering')

A=mean(mean(a));                          %taking the mean of the original image
B=mean(mean(n_highpass));                 %taking the mean of the high pass filtered image 

scalingfactor= A/B;

h2new=scalingfactor*h2                    %multiplying the the scaling factor to the new h2

y4=conv2(a,h2);
figure(5)
imshow(uint8(y4))
title('Y4')                               %Filtered image output Y4

figure(6)
k=imadd(y2,n_highpass,'uint8');
imshow(k,[])
title('Reconstructed Image')              %Reconstructed image output 
                 



