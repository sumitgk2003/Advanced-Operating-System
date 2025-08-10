#include<fcntl.h>
#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
using namespace std;

void blockReversal(string fileName,size_t blockSize){

}
void fullReversal(string fileName){

}
void partialRangeReversal(string fileName,off_t start,off_t end){

}


int main(int arg_count,char* arg_value[]){
    cout<<arg_count<<endl;
    if(arg_count<3){
        cout<<"Very less arguments";
        return 1;
    }
    for(int i=0;i<arg_count;i++){
        cout<<arg_value[i]<<endl;
    }
    long long int flag =strtoll(arg_value[2],NULL,10);
    string fileName = arg_value[1];
    cout<<fileName<<endl;
    cout<<arg_value[2]<<endl;
    if(flag==0){
        if(arg_count!=4){
            cout<<"Wrong number of arguments for flag 0";
            return 1;
        }
    }else if(flag==1){
        if(arg_count!=3){
            cout<<"Wrong number of arguments for flag 1";
            return 1;
        }
    }else if(flag==2){
        if(arg_count!=5){
            cout<<"Wrong number of arguments for flag 2";
            return 1;
        }
    }else{
        cout<<"Flag value is not valid";
        return 1;
    }

    if(mkdir("Assignment1",0700)==-1){
        if(errno=EEXIST){
            cout<<"Directory Already Exists"<<endl;
        }else{
            cout<<"mkdir failed";
            return 1;
        }
    }else{
        cout<<"Directory Created Successfully"<<endl;
    }

    int fd=open(fileName.c_str(),O_RDONLY);
    char buffer[1024];
    ssize_t bytes=read(fd,buffer,1024);
    string outputPath="Assignment1/"+to_string(flag)+"_"+fileName;
    int out=open(outputPath.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(out==-1)cout<<"Error while file creating";
    
    
    for(int i=0,j=strlen(buffer)-1;i<strlen(buffer)/2;i++,j--){
        char temp=buffer[i];
        buffer[i]=buffer[j];
        buffer[j]=temp;
    }
    write(out,buffer,1024);
    close(fd);
    close(out);

}