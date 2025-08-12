#include <sys/stat.h>
#include <unistd.h>
#include<iostream>
#include<fcntl.h>
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

void verify0(int oldFileDescriptor,int newFileDescriptor){

}

void verify1(){

}

void verify2(){

}

int main(int arg_count,char* arg_value[]){
    cout<<arg_count<<endl;
    if(arg_count<5){
        cout<<"Very less arguments";
        return 1;
    }
    for(int i=0;i<arg_count;i++){
        cout<<arg_value[i]<<endl;
    }
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