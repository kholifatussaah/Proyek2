#include <iostream>
#include <fstream> 
#include <iomanip>
#include <conio.h>
 
using namespace std;
 
class item{
    //Data members
private:
    int code;
    char name[15];
    int qty;
    //member function
public:
    void put_item(void);
    void get_item(void);
    int get_code (void){
        return code;
    };
 
    int update_qty(int num){
        qty = qty-num;
        return qty;
    }
 
    void inputCode(){
    while (!(cin >> code))
        {
            cout << "ERROR: a number must be entered: ";
            cin.clear();
            cin.ignore(132, '\n');
        }
}
 
};
 
//Member fungtion class item di luar class
 
void item::get_item(void){
        cout << "Masukkan Kode Item: ";
        inputCode();
        //fflush(stdin);
        cout << "Masukkan Nama Item: "; cin >> name;
        cout << "Masukkan Qty: "; cin >> qty;
    }
 
void item::put_item(void){
    cout<<setw(6)<<code<<setw(15)<<name<<setw(6)<<qty<<endl; //setup dengan lebar field membutuhkan library iomanip
}
 
//Fuction Prototype
void addBarang(void);
void showAll(void);
void showBarang(void);
void deleteNamaBarang(void);
void modifBarang(void);
 
//Global Declaration
item it;        //membuat object item
fstream file;   //membuat object fstream
 
int main()
{
    int option;
    while(1){
        system("cls");
        cout<<"***MENU***"<<endl;
        cout<<"1. Tambah Barang"<<endl;
        cout<<"2. Tampil Semua Data"<<endl;
        cout<<"3. Cari Barang Tertentu"<<endl;
        cout<<"4. Hapus Nama Barang"<<endl;
        cout<<"5. Ubah Barang"<<endl;
        cout<<"6. Keluar"<<endl;
        cout<<"Masukkan nomor opsi: "; cin>>option;
        switch(option){
            case 1:{
                addBarang();
                cout<<"Tekan sembarang untuk kembali ke menu ...";
                getch(); //butuh library conio.h
                break;
            }
            case 2:{
                cout<<"-----------------------------------------\n";
                cout<<setw(6)<<"Kode"<<setw(15)<<"Nama Barang"<<setw(6)<<"Qty"<<endl;
                cout<<"-----------------------------------------\n";
                showAll();
                cout<<"-----------------------------------------\n";
                cout<<"Tekan sembarang untuk kembali ke menu ...";
                getch();
                break;
            }
            case 3:{
                showBarang();
                cout<<"Tekan sembarang untuk kembali ke menu ...";
                getch();
                break;
            }
            case 4:{
                deleteNamaBarang();
                cout<<"Tekan sembarang untuk kembali ke menu ...";
                getch();
                break;
            }
            case 5:{
                modifBarang();
                cout<<"Tekan sembarang untuk kembali ke menu ...";
                getch();
                break;
            }
            case 6:{
                exit(0);
            }
            default:{
                cout<<"Opsi pilihan tidak ada, Tekan sembarang untuk kembali ke menu ...";
                getch();
                break;
            }
        }
    }
 
    return 0;
}
 
void addBarang(){
    char ch = 'y';
    file.open("stock.dat",ios::app|ios::binary);
    while(ch=='y'||ch=='Y'){
        it.get_item();
        //write object into file
        file.write((char*)&it, sizeof(it));
        cout<<"Tambah Barang lagi ... (y/n)?";
        cin>>ch;
    }
    file.close();
}
 
void showAll(){
    file.open("stock.dat", ios::in|ios::binary);
    if(!file)
    {
        cout <<"File not Found";
        exit(0);
    }else{
        file.read((char*)&it, sizeof(it));
        while (!file.eof()){
            it.put_item();
            file.read((char*)&it, sizeof(it));
        }
    }
    file.close();
}
 
void showBarang(void){
    int no, flag = 0;
    file.open("stock.dat", ios::in|ios::binary);
    if (!file){ //Jika file tidak bisa dibuka
        cout<<"file tidak ditemukan";
        exit(0);
    }else{
        cout<<"Masukkan code yang dicari: ";
        cin>>no;
        //Baca record dari file dan dimasukkan ke object
        while(!file.eof()){ //ketika akhir dari file
            if(no==it.get_code()){
                flag=1;
                cout<<"-----------------------------------------\n";
                cout<<setw(6)<<"Kode"<<setw(15)<<"Nama"<<setw(6)<<"Qty"<<endl;
                cout<<"-----------------------------------------\n";
                it.put_item();
                cout<<"-----------------------------------------\n";
                break;
            }
            file.read((char*)&it, sizeof(it));
        }
        if (flag==0){
            cout<<"Item tidak ditemukan ....\n";
        }
    }
    file.close();
}

void deleteNamaBarang(void){
    int no;
    cout<<"Masukkan kode barang untuk dihapus: "  ;
    cin>>no;
    ofstream file2;  //stream object
    //open new.dat file for write operation
    file2.open("new.dat",ios::binary);
    file.open("stock.dat", ios::in|ios::binary);
    if(!file)    //if open file fails
    {
        cout<<"file not found";
        exit(0);
    }else{
        //read record form stock.dat file into object
        file.read((char*)&it, sizeof(it));
        while(!file.eof()){
            if(no != it.get_code()){
                file2.write((char*)&it, sizeof(it));
            }
            file.read((char*)&it, sizeof(it));
        }
    }
    file2.close();
    file.close();
    remove("stock.dat");
    rename("new.dat","stock.dat");
}

void modifBarang(void){
    int no, num;
    cout<<"Masukkan kode barang untuk dirubah: "; cin>>no;
    cout<<"Masukkan Jumlah barang yang akan di keluarkan: "; cin>>num;
    file.open("stock.dat", ios::in|ios::out|ios::binary);
    if(!file){
        cout<<"File not found";
        exit(0);
    }
    while(file.read((char*)&it, sizeof(it))){
        if(it.get_code()==no){
            it.update_qty(num);
            int pos = sizeof(it);
            file.seekp(-pos, ios::cur);
            file.write((char*)&it, sizeof(it));
        }
    }
    file.close();
}
