# UESTC_compilers2019  
电子科技大学UESTC编译技术实验2019  
author：@newhans @JimHeisenberg  
第一次实验代码已经上传啦  
branch里有@JimHeisenberg同学写的windowsVersion  
还在用win的同学可以康康！  

# update
10.16日，@JimHeisenberg在windowsVersion中更新了更加优秀的auto_lex.l

windowsVersion也可以在linux上运行

# environment for windows
flex-2.5.4a-1  
gcc version 8.2.0  

# environment for linux
ubuntu 18.04  
flex 2.6.4  
gcc version 7.4.0  

# usage
flex auto_lex.l  
gcc lex.yy.c -o test -lfl  

# reference
(https://pandolia.net/tinyc/ch1_overview.html)   
(https://sighingnow.github.io/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86/lex_yacc.html)      
(https://juejin.im/post/5b5db5b8e51d4519155720d2)      
(https://segmentfault.com/a/1190000011674745)     
(https://jin-yang.github.io/post/program-concept-lexical-flex-introduce.html)    

# LICENSE  
## MIT  

