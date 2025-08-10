#include<fcntl.h>
#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
using namespace std;

void blockReversal(int inputFileDescriptor,int outputFileDescriptor,string fileName,ssize_t blockSize,off_t fileSize){
    off_t remSize=fileSize;
    off_t pointer=lseek(inputFileDescriptor,0,SEEK_SET);
    char buffer[blockSize+1];
    ssize_t bytesRead;
    while(remSize>0){
        bytesRead=min(blockSize,remSize);
        ssize_t bytes=read(inputFileDescriptor,buffer,bytesRead);
        //cout<<buffer<<endl;
        buffer[bytes]='\0';
        for(int i=0,j=bytesRead-1;i<bytesRead/2;i++,j--){
            char temp=buffer[i];
            buffer[i]=buffer[j];
            buffer[j]=temp;
        }
        write(outputFileDescriptor,buffer,bytesRead);  
        remSize=remSize-bytesRead;
        cout<<"remSize:"<<remSize<<endl;
        //lseek(inputFileDescriptor,bytesRead,SEEK_CUR);
        //buffer[0]='\0';
    }
    close(inputFileDescriptor);
    close(outputFileDescriptor);
}
void fullReversal(int inputFileDescriptor,int outputFileDescriptor,string fileName,off_t fileSize){

}
void partialRangeReversal(int inputFileDescriptor,int outputFileDescriptor,string fileName,off_t start,off_t end,off_t fileSize){

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
    off_t fileSize=lseek(fd,0,SEEK_END);
    cout<<"fileSize:"<<fileSize<<endl;
    if(fileSize==0){
        cout<<"File is empty"<<endl;
        return 1;
    }

    ssize_t blockSize;
    if(flag==0){
        blockSize=strtoll(arg_value[3],NULL,10);
        if(blockSize<=0){
            cout<<"Blocksize is not valid"<<endl;
            return 1;
        }
    }
    cout<<"blockSize:"<<blockSize<<endl;
    off_t start,end;
    if(flag==2){   
        start=strtoll(arg_value[3],NULL,10);
        end=strtoll(arg_value[4],NULL,10);
        if(start<0 ||end<0){
            cout<<"Index can't be negative"<<endl;
            return 1;
        }
        if(start>fileSize||end>fileSize){
            cout<<"Index can't be bigger than file size"<<endl;
            return 1;
        }
    }
    
    //char buffer[1024];
    //ssize_t bytes=read(fd,buffer,1024);
    
    string outputPath="Assignment1/"+to_string(flag)+"_"+fileName;
    int out=open(outputPath.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(out==-1)cout<<"Error while file creating";
    
    if(flag==0)blockReversal(fd,out,fileName,blockSize,fileSize);
    if(flag==1)fullReversal(fd,out,fileName,fileSize);
    if(flag==2)partialRangeReversal(fd,out,fileName,start,end,fileSize);
    
    // for(int i=0,j=strlen(buffer)-1;i<strlen(buffer)/2;i++,j--){
    //     char temp=buffer[i];
    //     buffer[i]=buffer[j];
    //     buffer[j]=temp;
    // }
    // write(out,buffer,1024);
    // close(fd);
    // close(out);

}