#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

void B_Sort(int a[], int s){
    for(int i = 0; i< s-1; i++){
        for(int j= 0; j< s-2-i; j++){
            if(a[j] < a[j+1]){
                int T = a[j];
                a[j] = a[j+1];
                a[j+1] = T;
            }
        }
    }
}

struct PGMImage{
    int H, W, MaxGray;
    string Comment;
    string MagicNo;
    vector<vector<int>> ImageData;

    PGMImage(){
        cout<<"Called Automatically"<<endl;
        H = W = MaxGray = -1;
    }

    void Create(vector<vector<int>>& Img, int H, int W){
        Img = vector<vector<int>>(H);
        for(int i = 0; i< H; i++)
            Img[i] = vector<int>(W);
        return;
    }

    bool ReserveMemory(){
        // Code to create memory storage for the image
        // This code assumes that H and W have been set earlier
        cout<<W<<" "<<H<<endl;
        if(H < 0 || W < 0)
            return false;
        Create(ImageData, H, W);
        return true;
    }

    bool LoadImage(char FileName[]){
        // Code to open File, Reserve Memory and load image data in memory
        ifstream FIN(FileName);
        if(! FIN.is_open())
            return false;

        getline(FIN, MagicNo);
        getline(FIN, Comment); // reading the comment. PROBLEM HERE THE COMMENTIS OPTIONAL

        FIN >> W >> H >> MaxGray;
        if( !ReserveMemory()){
            FIN.close();
            return false;
        }

        for(int r = 0; r < H; r++){
            for(int c = 0; c < W; c++){
                FIN >> ImageData[r][c];
            }
        }

        FIN.close();
        return true;
    }

    bool SaveImage(char FileName[]){
        // Code to open File, Reserve Memory and load image data in memory
        if(H < 0)
            return false;

        ofstream FOUT(FileName);
            // Your Code to check if the file has been created for output
            // If File not created then return false

        FOUT << "P2" <<endl
             << "# Created By MB For BDS-1A and BDS-1C"<<endl
             << W <<" " <<H<<endl
             <<MaxGray<<endl;

        for(int r = 0; r < H; r++){
            for(int c = 0; c < W; c++){
                FOUT << ImageData[r][c]<<" ";
            }
            FOUT << endl;
        }
        FOUT.close();
        return true;
    }

    bool Create_Negative(){
        if(H < 0)
            return false;
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W; c++)
                ImageData[r][c] = MaxGray - ImageData[r][c];
        }
        return true;
    }

    bool Mean_Filter(int FilterSize=3){
        // create a new image in memory
        // apply median filter and then copy the result
        // back to the same image
        if(H < 0)
            return false;
        vector<vector<int>> Temp;
        Create(Temp, H, W);
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W ; c++){
                int k = 0;
                int SUM = 0;
                for(int i=r-FilterSize/2; i< r+FilterSize/2; i++){
                    for(int j= c-FilterSize/2; j< c+FilterSize/2; j++){
                        if(i >=0 && i < H && j >= 0 && j < W){
                            SUM+= ImageData[i][j];
                            k++;
                        }
                    }
                }
                Temp[r][c] = SUM/k;
            }
        }
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W ; c++){
                ImageData[r][c] = Temp[r][c];
            }
        }
    }

    bool Median_Filter(int FilterSize=3){
        // create a new image in memory
        // apply median filter and then copy the result
        // back to the same image
        if(H < 0)
            return false;
        vector<vector<int>> Temp;
        Create(Temp, H, W);
        int D[100];
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W ; c++){
                int k = 0;
                for(int i=r-FilterSize/2; i< r+FilterSize/2; i++){
                    for(int j= c-FilterSize/2; j< c+FilterSize/2; j++){
                        if(i >=0 && i < H && j >= 0 && j < W){
                            D[k] = ImageData[i][j];
                            k++;
                        }
                    }
                }
                B_Sort(D, k);
                Temp[r][c] = D[k/2];
            }
        }
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W ; c++){
                ImageData[r][c] = Temp[r][c];
            }
        }
        return true;
    }

    bool FlipHorizontal(){
        if(H < 0)
            return false;
        int t;
        for(int indexrow=0; indexrow<H; indexrow++)
        {
            for(int indexcol=0;indexcol<W/2;indexcol++)
            {
                t=ImageData[indexrow][indexcol];
                ImageData[indexrow][indexcol] = ImageData[indexrow][W-1-indexcol];
                ImageData[indexrow][W-1-indexcol] = t;
            }
        }
        return true;
    }

    bool FlipVertical(){
        if(H<0)
            return false;
        int t;
        for(int indexcol=0;indexcol<W;indexcol++)
        {
            for(int indexrow=0; indexrow<H/2; indexrow++)
            {
                t=ImageData[indexrow][indexcol];
                ImageData[indexrow][indexcol] = ImageData[H-1-indexrow][indexcol];
                ImageData[H-1-indexrow][indexcol] = t;
            }
      }

        return true;
    }

    bool RotateClockwise(){
        for (int ind_rws = 0; ind_rws < H / 2; ind_rws++)
        {
            for (int ind_col = ind_rws; ind_col < H - ind_rws - 1; ind_col++)
            {
                int temp = ImageData[ind_rws][ind_col];
                ImageData[ind_rws][ind_col] = ImageData[H - 1 - ind_col][ind_rws];
                ImageData[H - 1 - ind_col][ind_rws] = ImageData[H - 1 - ind_rws][H - 1 - ind_col];
                ImageData[H - 1 - ind_rws][H - 1 - ind_col] = ImageData[ind_col][H - 1 - ind_rws];
                ImageData[ind_col][H - 1 - ind_rws] = temp;
            }
        }

        return true;
    }

    bool RotateCounterClockwise(){
        if (H<0)
            return false;
    for (int ind_rws = 0; ind_rws < H / 2; ind_rws++)
        {
            for (int ind_col = ind_rws; ind_col < H - ind_rws - 1; ind_col++)
            {
                int temp = ImageData[ind_rws][ind_col];
                ImageData[ind_rws][ind_col] = ImageData[ind_col][H - 1 - ind_rws];
                ImageData[ind_col][H - 1 - ind_rws] = ImageData[H - 1 - ind_rws][H - 1 - ind_col];
                ImageData[H - 1 - ind_rws][H - 1 - ind_col] = ImageData[H - 1 - ind_col][ind_rws];
                ImageData[H - 1 - ind_col][ind_rws] = temp;
            }
        }
        return true;
    }

    bool Rotate(double angle){
        vector<vector<int>> t;
        Create(t, H, W);
        for(int r=0;r<H;r++)
        {
            for(int c=0;c<W;c++)
            {
                int row=(r*cos(angle))-(c*sin(angle));
                int col=(r*sin(angle))+(c*cos(angle));
                if(row>0 && row<H && col>0 && col<W)
                t[row][col]=ImageData[r][c];
            }
        }
        for(int r=0;r<H;r++)
        {
            for(int c=0;c<W;c++)
            {
                ImageData[r][c]=t[r][c];
            }
        }

        return true;
    }

    bool Resize(int NewH, int NewW){
        if(H<0)
            return false;
       if(NewH!=0 || NewW!=0)
       {
            H=NewH;
            W=NewW;
       }
       ImageData.resize(NewH, vector<int>(NewW));
            return true;
    }

    bool CropImage(int Left, int Top, int Right, int Bottom){

        int NewH, NewW;
        NewH = H - Top + Bottom;
        NewW = W - Right + Left;
        if (H < 0)
            return false;
        vector<vector<int>> tmp;
        Create(tmp, NewH, NewW);

        for (int rws = Top; rws < H - Bottom; rws++)
        {
            for (int cols = Left; cols < W - Right; cols++)
            {
                tmp[rws][cols] = ImageData[rws][cols];
            }
        }
        Create(ImageData, NewH, NewW);
        for (int rws = 0; rws < NewH; rws++)
        {
            for (int cols = 0; cols < NewW; cols++)
            {
                ImageData[rws][cols] = tmp[rws][cols];
            }
        }
        H = NewH;
        W = NewW;
        return true;
    }


};

int Menu(){
    int choice;
    do{
        cout<<endl<<"1.  Load Image"<<endl
        <<"2.  Save Image"<<endl
        <<"__________________________"<<endl
        <<"3.  Create Negative Image"<<endl
        <<"__________________________"<<endl
        <<"4.  Mean Filter"<<endl
        <<"5.  Median Filter"<<endl
        <<"__________________________"<<endl
        <<"6.  Flip Horizontal"<<endl
        <<"7.  Flip Vertical"<<endl
        <<"8.  Rotate Clockwise by 90 Degree"<<endl
        <<"9.  Rotate Counter Clockwise by 90 degree"<<endl
        <<"10. Rotate By arbitrary angle"<<endl
        <<"11. Resize Image"<<endl
        <<"12. Crop Image"<<endl
        <<"__________________________"<<endl
        <<"13. Exit"<<endl<<endl
        <<"Enter Your Choice: ";
        cin>> choice;
    } while(choice < 1 || choice > 13);
}

int main()
{
    PGMImage Image;

    char FileName[100];
    int Choice;

    do{
        Choice = Menu();
        if(Choice == 1){
            cout<<"Enter File Name ";
            cin>>FileName;
            if(Image.LoadImage(FileName) )
               cout<<"Image Loaded Successfully "<<endl;
            else
                cout<<"Image Not Loaded Successfully "<<endl;
        }
        else if(Choice == 2){
            cout<<"Enter File Name ";
            cin >> FileName;
            if(Image.SaveImage(FileName) )
               cout<<"Image Saved Successfully "<<endl;
            else
                cout<<"Image Not Saved Successfully "<<endl;

        }
        else if(Choice == 3){
            if(Image.H < 0)
                cout<<"No Image Loaded"<<endl;
            else
                Image.Create_Negative();
        }
        else if(Choice == 4){
            Image.Mean_Filter(5);
        }
        else if(Choice == 5){
            int S;
            cout<<"Filter Size ";
            cin>> S;
            Image.Median_Filter(S);
        }
        else if(Choice == 6){
            Image.FlipHorizontal();
        }
        else if(Choice == 7){
            Image.FlipVertical();
            }
        else if(Choice == 8){
            Image.RotateClockwise();
        }
        else if(Choice == 9){
            Image.RotateCounterClockwise();
        }
        else if(Choice == 10){
        double angle;
        cout<<"Enter the angle ";
        cin>>angle;
        Image.Rotate(angle);

        }
        else if(Choice == 11){
            int height, width;
            cout<<"Enter new Height "<<endl;
            cin>>height;
            cout<<"Enter new Width "<<endl;
            cin>>width;
            Image.Resize(height, width);
        }
        else if(Choice == 12){
            int Left, Top, Right, Bottom;
            cout<<"Enter the number of lines you want to crop from left side ";
            cin>>Left;
            cout<<"Enter the number of lines you want to crop from the top ";
            cin>>Top;
            cout<<"Enter the number of lines you want to crop from right side ";
            cin>>Right;
            cout<<"Enter the number of lines you want to crop from the bottom ";
            cin>>Bottom;
            Image.CropImage(Left, Top, Right, Bottom);
        }

    }while(Choice != 13);

    return 0;
}
