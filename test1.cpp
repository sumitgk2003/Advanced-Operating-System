#include <sys/stat.h>
#include <unistd.h>
#include<iostream>
#include<fcntl.h>
#include<string.h>
using namespace std;

void permissionChecker(struct stat stat,string s){
    cout<<"User has read permissions on "<<s<<": ";
    if(stat.st_mode & S_IRUSR)cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
    cout<<"User has write permission on "<<s<<": ";
    if(stat.st_mode & S_IWUSR)cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
    cout<<"User has execute permission on "<<s<<": ";
    if(stat.st_mode & S_IXUSR)cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
    cout<<"Group has read permissions on "<<s<<": ";
    if(stat.st_mode & S_IRGRP)cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
    cout<<"Group has write permission on "<<s<<": ";
    if(stat.st_mode & S_IWGRP)cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
    cout<<"Group has execute permission on "<<s<<": ";
    if(stat.st_mode & S_IXGRP)cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
    cout<<"Others has read permissions on "<<s<<": ";
    if(stat.st_mode & S_IROTH)cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
    cout<<"Others has write permission on "<<s<<": ";
    if(stat.st_mode & S_IWOTH)cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
    cout<<"Others has execute permission on "<<s<<": ";
    if(stat.st_mode & S_IXOTH)cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
}

bool verify0(int oldFileDescriptor,int newFileDescriptor,off_t blockSize,off_t fileSize){
    off_t remSize=fileSize;
    off_t pointer=lseek(oldFileDescriptor,0,SEEK_SET);
    off_t pointer1=lseek(newFileDescriptor,0,SEEK_SET);
    char oldBuffer[blockSize+1];
    char newBuffer[blockSize+1];
    //char buffer[blockSize+1];
    ssize_t bytesRead,newBytesRead;
    while(remSize>0){
        bytesRead=min(blockSize,remSize);
        ssize_t bytes=read(oldFileDescriptor,oldBuffer,bytesRead);
        ssize_t newBytes=read(newFileDescriptor,newBuffer,bytesRead);
        if(newBytes==-1){
            cout<<"error while reading the new buffer";
        }
        cout<<bytes<<endl;
        cout<<newBytes<<endl;
        oldBuffer[bytes]='\0';
        newBuffer[newBytes]='\0';
        for(int i=0,j=bytesRead-1;i<bytesRead/2;i++,j--){
            char temp=oldBuffer[i];
            oldBuffer[i]=oldBuffer[j];
            oldBuffer[j]=temp;
        } 
        cout<<oldBuffer<<endl;
        cout<<newBuffer<<endl;
        if(strcmp(oldBuffer,newBuffer)!=0)return false;
        remSize=remSize-bytesRead;
    }
    return true;
    //cout<<endl;
}

bool verify1(int oldFileDescriptor,int newFileDescriptor,off_t fileSize){
    ssize_t blockSize=1e6;
    //char buffer[block];
    off_t remSize=fileSize;
    //off_t pointer=lseek(oldFileDescriptor,0,SEEK_SET);
    off_t pointer1=lseek(newFileDescriptor,0,SEEK_SET);
    char oldBuffer[blockSize+1];
    char newBuffer[blockSize+1];
    //char buffer[blockSize+1];
    //ssize_t bytesRead,newBytesRead;
    if(blockSize<fileSize)
    off_t pointer=lseek(oldFileDescriptor,-blockSize,SEEK_END);
    else 
    off_t pointer=lseek(oldFileDescriptor,0,SEEK_SET);
    //off_t remSize=fileSize;
    ssize_t bytesRead;
    while(remSize>0){
        bytesRead=min(blockSize,remSize);
        ssize_t bytes=read(oldFileDescriptor,oldBuffer,bytesRead);
        ssize_t newBytes=read(newFileDescriptor,newBuffer,bytesRead);
        //cout<<buffer<<endl;
        oldBuffer[bytes]='\0';
        newBuffer[newBytes]='\0';
        for(int i=0,j=bytesRead-1;i<bytesRead/2;i++,j--){
            char temp=oldBuffer[i];
            oldBuffer[i]=oldBuffer[j];
            oldBuffer[j]=temp;
        }
        //write(outputFileDescriptor,buffer,bytesRead);  
        remSize=remSize-bytesRead;
        //cout<<oldBuffer<<endl;
        //cout<<newBuffer<<endl;
        //cout<<"\r\033[2K"<<"progress:"<<double(fileSize-remSize)*100/double(fileSize)<<flush;
        if(remSize>=blockSize){
            lseek(oldFileDescriptor,-2*blockSize,SEEK_CUR);
        }else{
            lseek(oldFileDescriptor,0,SEEK_SET);
        }
        if(strcmp(oldBuffer,newBuffer)!=0){
            return false;
        }
    }
    return true;
    //cout<<endl;
}

bool verify2(int oldFileDescriptor,int newFileDescriptor,off_t fileSize,off_t start,off_t end){
    ssize_t block=1e6;
    off_t firstBlockSize=start;
    char oldBuffer[block+1];
    char newBuffer[block+1];
    off_t pointer1=lseek(newFileDescriptor,0,SEEK_SET);
    if(block<firstBlockSize)
    off_t pointer=lseek(oldFileDescriptor,start-block,SEEK_SET);
    else 
    off_t pointer=lseek(oldFileDescriptor,0,SEEK_SET);
    off_t remSize=firstBlockSize;
    ssize_t bytesRead;
    while(remSize>0){
        bytesRead=min(block,remSize);
        ssize_t bytes=read(oldFileDescriptor,oldBuffer,bytesRead);
        ssize_t newBytes=read(newFileDescriptor,newBuffer,bytesRead);
        //cout<<oldBuffer<<endl;
        oldBuffer[bytes]='\0';
        newBuffer[newBytes]='\0';
        for(int i=0,j=bytesRead-1;i<bytesRead/2;i++,j--){
            char temp=oldBuffer[i];
            oldBuffer[i]=oldBuffer[j];
            oldBuffer[j]=temp;
        }
        //write(outputFileDescriptor,oldBuffer,bytesRead);  
        remSize=remSize-bytesRead;
        //cout<<"\r\033[2K"<<"progress of first block:"<<double(firstBlockSize-remSize)*100/double(firstBlockSize)<<flush;
        if(strcmp(oldBuffer,newBuffer)!=0){
            
            return false; 
        }
        if(remSize>=block){
            lseek(oldFileDescriptor,-2*block,SEEK_CUR);
        }else{
            lseek(oldFileDescriptor,0,SEEK_SET);
        }
    }
    //cout<<endl;
    off_t secondBlockSize=end-start+1;
    off_t pointer=lseek(oldFileDescriptor,start,SEEK_SET);
    remSize=secondBlockSize;
    while(remSize>0){
        bytesRead=min(block,remSize);
        ssize_t bytes=read(oldFileDescriptor,oldBuffer,bytesRead);
        ssize_t newBytes=read(newFileDescriptor,newBuffer,bytesRead);
        //cout<<oldBuffer<<endl;
        oldBuffer[bytes]='\0';
        newBuffer[newBytes]='\0';
        //write(outputFileDescriptor,oldBuffer,bytesRead);  
        remSize=remSize-bytesRead;
        if(strcmp(oldBuffer,newBuffer)!=0){
            
            return false;
        }
        //cout<<"\r\033[2K"<<"progress of second block:"<<double(secondBlockSize-remSize)*100/double(secondBlockSize)<<flush;
    }
    //cout<<endl;
    off_t thirdBlockSize=fileSize-end-1;
    //fullReversal(oldFileDescriptor,outputFileDescriptor,thirdBlockSize);
    if(block<thirdBlockSize)
    off_t pointer=lseek(oldFileDescriptor,-block,SEEK_END);
    else 
    off_t pointer=lseek(oldFileDescriptor,end+1,SEEK_SET);
    remSize=thirdBlockSize;
    while(remSize>0){
        bytesRead=min(block,remSize);
        ssize_t bytes=read(oldFileDescriptor,oldBuffer,bytesRead);
        ssize_t newBytes=read(newFileDescriptor,newBuffer,bytesRead);
        //cout<<oldBuffer<<endl;
        oldBuffer[bytes]='\0';
        newBuffer[newBytes]='\0';
        for(int i=0,j=bytesRead-1;i<bytesRead/2;i++,j--){
            char temp=oldBuffer[i];
            oldBuffer[i]=oldBuffer[j];
            oldBuffer[j]=temp;
        }
        //write(outputFileDescriptor,buffer,bytesRead);  
        remSize=remSize-bytesRead;
        //cout<<"\r\033[2K"<<"progress of third block:"<<double(thirdBlockSize-remSize)*100/double(thirdBlockSize)<<flush;
        if(strcmp(oldBuffer,newBuffer)!=0){
            
            return false; 
        }
        if(remSize>=block){
            lseek(oldFileDescriptor,-2*block,SEEK_CUR);
        }else{
            lseek(oldFileDescriptor,0,SEEK_SET);
        }
    }
    return true;
    //cout<<endl;
}

int main(int arg_count,char* arg_value[]){
    //cout<<arg_count<<endl;
    if(arg_count<5){
        cout<<"Very less arguments";
        return 1;
    }
    // for(int i=0;i<arg_count;i++){
    //     cout<<arg_value[i]<<endl;
    // }
    long long int flag =strtoll(arg_value[4],NULL,10);
    string newFilePath = arg_value[1];
    string oldFilePath = arg_value[2];
    string directoryPath = arg_value[3];
    //cout<<fileName<<endl;
    //cout<<arg_value[2]<<endl;
    if(flag==0){
        if(arg_count!=6){
            cout<<"Wrong number of arguments for flag 0";
            return 1;
        }
    }else if(flag==1){
        if(arg_count!=5){
            cout<<"Wrong number of arguments for flag 1";
            return 1;
        }
    }else if(flag==2){
        if(arg_count!=7){
            cout<<"Wrong number of arguments for flag 2";
            return 1;
        }
    }else{
        cout<<"Flag value is not valid";
        return 1;
    }

    struct stat oldFileStat;
    if(stat(oldFilePath.c_str(),&oldFileStat)<0){
        perror("stat");
        cout<<"old file path is incorrect"<<endl;
        return 1;
    };
    struct stat newFileStat;
    if(stat(newFilePath.c_str(),&newFileStat)<0){
        perror("stat");
        cout<<"new file path is incorrect"<<endl;
        return 1;
    };
    struct stat directoryStat;
    cout<<"Directory is created: ";
    if(stat(directoryPath.c_str(),&directoryStat)<0){
        cout<<"No"<<endl;
        return 1;
    }else{
        cout<<"Yes"<<endl;
    };
    int oldFileDescriptor=open(oldFilePath.c_str(),O_RDONLY);
    off_t oldFileSize=lseek(oldFileDescriptor,0,SEEK_END);
    int newFileDescriptor=open(newFilePath.c_str(),O_RDONLY);
    off_t newFileSize=lseek(newFileDescriptor,0,SEEK_END);
    ssize_t blockSize;
    if(flag==0){
        blockSize=strtoll(arg_value[5],NULL,10);
        if(blockSize<=0){
            cout<<"Blocksize is not valid"<<endl;
            return 1;
        }
    }
    //cout<<"blockSize:"<<blockSize<<endl;
    off_t start,end;
    if(flag==2){   
        start=strtoll(arg_value[5],NULL,10);
        end=strtoll(arg_value[6],NULL,10);
        if(start<0 ||end<0){
            cout<<"Index can't be negative"<<endl;
            return 1;
        }
        if(start>oldFileSize||end>oldFileSize){
            cout<<"Index can't be bigger than file size"<<endl;
            return 1;
        }
    }
    cout<<"Whether file contents are correctly processed: ";
    if(flag==0){
        if(verify0(oldFileDescriptor,newFileDescriptor,blockSize,oldFileSize))
            cout<<"Yes"<<endl;
        else
            cout<<"No"<<endl;
    }else if(flag==1){
        if(verify1(oldFileDescriptor,newFileDescriptor,oldFileSize))
            cout<<"Yes"<<endl;
        else
            cout<<"No"<<endl;
    }else if(flag==2){
        if(verify2(oldFileDescriptor,newFileDescriptor,oldFileSize,start,end))
            cout<<"Yes"<<endl;
        else
            cout<<"No"<<endl;
    }
    
    cout<<"Both Files Sizes are Same: ";
    if(oldFileSize==newFileSize){
        cout<<"Yes"<<endl;
    }else{
        cout<<"No"<<endl;
    }
    permissionChecker(newFileStat,"newFile");
    permissionChecker(oldFileStat,"oldFile");
    permissionChecker(directoryStat,"directory");
    
}