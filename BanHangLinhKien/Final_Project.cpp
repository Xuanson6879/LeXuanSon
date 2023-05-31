#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <time.h>

using namespace std;

struct Product
{
public:
    unsigned int ID_P;
    string NameProduc;
    string Input_P;
    string MadeIn;
    string Brand;
    float Weight;
    float Power;
    float Price;
    int Number;
};

struct Curstomer
{
public:
    int id_cust{};
    string name;
    string gentle;
    string bir_day{};
    int number;
    vector<Product> List_P;

    string getInfo() const
    {
        return "\t|\t\t" + to_string(id_cust) + "\t\t" +
               "|" + "\t\t" + name + "\t\t" +
               "|" + "\t\t" + gentle + "\t\t" +
               "|" + "\t\t" + bir_day + "\t\t" + "|" + "\n";
    }
};

class Curstomer_LinkedList
{
    struct Node // Khai báo 1 cấu trúc dữ liệu cho 1 node
    {
        Curstomer people;
        Node *p_next_;
    };

public:
    Curstomer_LinkedList() { p_head_ = NULL; }
    ~Curstomer_LinkedList()
    {
        Node *head = p_head_;
        while (head)
        {
            Node *temp = head;
            head = head->p_next_;
            delete temp;
            temp = NULL;
        }
    }
    vector<string> split(string stringToBeSplitted, char delimiter)
    {
        vector<string> splittedString;
        stringstream ss(stringToBeSplitted); // convert stringToBeSplitted into stringstream
        string item;
        // using getline() method to split string then save substrings in item base on delimeter
        while (getline(ss, item, delimiter))
        {
            splittedString.push_back(item); // push these items into splittedString
        }
        return splittedString;
    }
    void Loading_Data_PC()
    {
        ifstream fileIn;
        fileIn.open("Product_of_C.txt", ios::in); // open file
        string line;
        int id_c;
        while (getline(fileIn, line))
        {
            vector<string> result = split(line, '\t'); // slipt a line from file base on tab character
            // display informations of student
            char *cstr = new char[20];
            // char *p = strtok(cstr, " ");
            if (result[1] == "id_customer")
            {
                strcpy(cstr, result[0].c_str());
                id_c = atoi(cstr);
                delete[] cstr;
                continue;
            }
            Product temData;
            strcpy(cstr, result[0].c_str());
            temData.ID_P = atoi(cstr);
            strcpy(cstr, result[1].c_str());
            temData.NameProduc = cstr;
            strcpy(cstr, result[2].c_str());
            temData.Input_P = cstr;
            strcpy(cstr, result[3].c_str());
            temData.MadeIn = cstr;
            strcpy(cstr, result[4].c_str());
            temData.Brand = cstr;
            strcpy(cstr, result[5].c_str());
            temData.Weight = atof(cstr);
            strcpy(cstr, result[6].c_str());
            temData.Power = atof(cstr);
            strcpy(cstr, result[7].c_str());
            temData.Price = atof(cstr);
            strcpy(cstr, result[8].c_str());
            temData.Number = atoi(cstr);

            // AddValue(temData);
            Add_P_C(id_c, temData);

            delete[] cstr;
        }
        fileIn.close(); // close file
    }
    void UpLoading_Data_PC()
    {
        ifstream fileIn;
        fileIn.open("Product_of_C.txt");
        ofstream fileOut;
        fileOut.open("temps.txt");

        Node *temp = p_head_;
        while (temp != NULL)
        {
            Curstomer value = temp->people;
            fileOut << to_string(value.id_cust);
            fileOut << '\t';
            fileOut << "id_customer";
            fileOut << '\n';
            for (const auto &element : value.List_P)
            {
                fileOut << element.ID_P;
                fileOut << '\t';
                fileOut << element.NameProduc;
                fileOut << '\t';
                fileOut << element.Input_P;
                fileOut << '\t';
                fileOut << element.MadeIn;
                fileOut << '\t';
                fileOut << element.Brand;
                fileOut << '\t';
                fileOut << element.Weight;
                fileOut << '\t';
                fileOut << element.Power;
                fileOut << '\t';
                fileOut << element.Price;
                fileOut << '\t';
                fileOut << element.Number;
                fileOut << '\t';
                fileOut << '\n';
            }
            temp = temp->p_next_;
        }

        fileIn.close();
        fileOut.close();
        remove("Product_of_C.txt");
        rename("temps.txt", "Product_of_C.txt");
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    void Loading_Data_C()
    {

        ifstream fileIn;
        fileIn.open("Curstomer.txt", ios::in); // open file
        string line;

        while (getline(fileIn, line))
        {
            vector<string> result = split(line, '\t'); // slipt a line from file base on tab character
            // display informations of student
            char *cstr = new char[20];
            // char *p = strtok(cstr, " ");
            Curstomer temData;
            strcpy(cstr, result[0].c_str());
            temData.id_cust = atoi(cstr);
            strcpy(cstr, result[1].c_str());
            temData.name = cstr;
            strcpy(cstr, result[2].c_str());
            temData.gentle = cstr;
            strcpy(cstr, result[3].c_str());
            temData.bir_day = cstr;
            strcpy(cstr, result[4].c_str());
            temData.number = atoi(cstr);

            AddValue(temData);

            delete[] cstr;
        }
        fileIn.close(); // close file
    }
    void Uploading_Data_c()
    {
        ifstream fileIn;
        fileIn.open("Curstomer.txt");
        ofstream fileOut;
        fileOut.open("temps.txt");

        Node *temp = p_head_;
        while (temp != NULL)
        {
            Curstomer value = temp->people;
            fileOut << to_string(temp->people.id_cust);
            fileOut << '\t';
            fileOut << temp->people.name;
            fileOut << '\t';
            fileOut << temp->people.gentle;
            fileOut << '\t';
            fileOut << temp->people.bir_day;
            fileOut << '\t';
            fileOut << to_string(temp->people.number);
            fileOut << '\t';
            fileOut << '\n';
            temp = temp->p_next_;
        }
        fileIn.close();
        fileOut.close();
        remove("Curstomer.txt");
        rename("temps.txt", "Curstomer.txt");
    }

    void AddValue(Curstomer val) // Thêm 1 phần tử mới vào đầu danh sách
    {
        Node *new_node = new Node();
        new_node->people = val;
        new_node->p_next_ = p_head_;
        p_head_ = new_node;
    }

    bool Remove(int id) // Loại bỏ phần từ đầu danh sách
    {
        Node *temp = p_head_;
        while (temp != NULL)
        {
            Node *temp1 = temp->p_next_;
            if (temp1 != NULL)
            {
                if (temp1->people.id_cust == id)
                {
                    temp->p_next_ = temp1->p_next_;
                    delete temp1;
                    return true;
                }
            }
            temp = temp->p_next_;
        }
        return false;
    }

    void Show() // Viết thêm một hàm hiển thị
    {
        Node *temp = p_head_;
        while (temp != NULL)
        {
            Curstomer value = temp->people;
            cout << value.getInfo();
            temp = temp->p_next_;
        }
    }
    // void selection_soft()
    // {
    //     Node *temp = p_head_;
    //     while (temp != NULL)
    //     {
    //         if (temp->people.id_cust > temp->p_next_->people.id_cust)

    //             temp = temp->p_next_->people.id_cust;
    //     }
    // }
    void Add_P_C(int id_c, const Product item)
    {
        Node *temp = p_head_;
        while (temp != NULL)
        {

            if (temp->people.id_cust == id_c)
            {
                temp->people.List_P.push_back(item);
                break;
            }
            temp = temp->p_next_;
        }
    }
    // vector<Product> Get_P_C(int id_c, int &check)
    // {
    //     Node *temp = p_head_;
    //     while (temp != NULL)
    //     {

    //         if (temp->people.id_cust == id_c)
    //         {
    //             check = 1;
    //             return temp->people.List_P;
    //         }
    //         temp = temp->p_next_;
    //     }
    // }

private:
    Node *p_head_; // Luôn có 1 con trỏ pHead quản lý phần tử đầu.
};

class Tree_Mannager_System
{
private:
    struct Node
    {
        Product data;
        Node *left;
        Node *right;
    };
    Node *root;            // Luôn có 1 con trỏ pHead quản lý phần tử đầu.
    vector<Product> stack; // là 1 vector tạm để đưa tất cả sản phẩm lên data file.

public:
    Tree_Mannager_System() { root = NULL; }
    vector<string> split(string stringToBeSplitted, char delimiter)
    {
        vector<string> splittedString;
        stringstream ss(stringToBeSplitted); // convert stringToBeSplitted into stringstream
        string item;
        // using getline() method to split string then save substrings in item base on delimeter
        while (getline(ss, item, delimiter))
        {
            splittedString.push_back(item); // push these items into splittedString
        }
        return splittedString;
    }
    void Loading_Data()
    {
        system("cls");
        ifstream fileIn;
        fileIn.open("Data.txt", ios::in); // open file
        string line;

        while (getline(fileIn, line))
        {
            vector<string> result = split(line, '\t'); // slipt a line from file base on tab character
            // display informations of student
            char *cstr = new char[20];
            // char *p = strtok(cstr, " ");
            Product temData;
            strcpy(cstr, result[0].c_str());
            temData.ID_P = atoi(cstr);
            strcpy(cstr, result[1].c_str());
            temData.NameProduc = cstr;
            strcpy(cstr, result[2].c_str());
            temData.Input_P = cstr;
            strcpy(cstr, result[3].c_str());
            temData.MadeIn = cstr;
            strcpy(cstr, result[4].c_str());
            temData.Brand = cstr;
            strcpy(cstr, result[5].c_str());
            temData.Weight = atof(cstr);
            strcpy(cstr, result[6].c_str());
            temData.Power = atof(cstr);
            strcpy(cstr, result[7].c_str());
            temData.Price = atof(cstr);
            strcpy(cstr, result[8].c_str());
            temData.Number = atoi(cstr);
            // insert(temData);
            inserts(temData);

            delete[] cstr;
        }
        fileIn.close(); // close file
    }
    void Uploading_Data()
    {
        ifstream fileIn;
        fileIn.open("Data.txt");
        ofstream fileOut;
        fileOut.open("temps.txt");

        Node *temp = root;
        uploading(temp);
        for (const auto &element : stack)
        {
            fileOut << element.ID_P;
            fileOut << '\t';
            fileOut << element.NameProduc;
            fileOut << '\t';
            fileOut << element.Input_P;
            fileOut << '\t';
            fileOut << element.MadeIn;
            fileOut << '\t';
            fileOut << element.Brand;
            fileOut << '\t';
            fileOut << element.Weight;
            fileOut << '\t';
            fileOut << element.Power;
            fileOut << '\t';
            fileOut << element.Price;
            fileOut << '\t';
            fileOut << element.Number;
            fileOut << '\t';
            fileOut << '\n';
        }
        fileIn.close();
        fileOut.close();
        remove("Data.txt");
        rename("temps.txt", "Data.txt");
    }

    void inserts(Product item)
    {
        Node *p = new Node;
        Node *parent;
        p->data = item;
        p->left = NULL;
        p->right = NULL;
        parent = NULL;
        if (root == NULL)
            root = p;
        else
        {
            Node *ptr;
            ptr = root;
            while (ptr != NULL)
            {
                parent = ptr;
                // if (item & gt; ptr - &gt; data)
                if (item.ID_P > ptr->data.ID_P)
                    ptr = ptr->right;
                else
                    ptr = ptr->left;
            }
            if (item.ID_P < parent->data.ID_P)
                parent->left = p;
            else
                parent->right = p;
        }
    }

    Node *search(unsigned int ID_P)
    {
        Node *current = root;

        while (current->data.ID_P != ID_P)
        {
            // if (current != NULL)
            // printf("%d ", current->data);

            // chuyen toi cay con ben trai
            if (current->data.ID_P > ID_P)
            {
                current = current->left;
            }
            // chuyen toi cay con ben phai
            else
            {
                current = current->right;
            }

            // khong tim thay
            if (current == NULL)
            {
                return NULL;
            }
        }
        return current;
    }

    Product *Seaching(unsigned int id)
    {
        Node *temp = search(id);
        // Product *tep = temp->data;
        if (temp == NULL)
        {
            return NULL;
        }
        else

        {
            return &temp->data;
        }
    }
    /* Given a binary search tree and a key, this function
       deletes the key and returns the new root */
    Node *deleteNode(Node *root, unsigned int k)
    {
        // Base case
        if (root == NULL)
            return root;

        // Recursive calls for ancestors of
        // node to be deleted
        if (root->data.ID_P > k)
        {
            root->left = deleteNode(root->left, k);
            return root;
        }
        else if (root->data.ID_P < k)
        {
            root->right = deleteNode(root->right, k);
            return root;
        }

        // We reach here when root is the node
        // to be deleted.

        // If one of the children is empty
        if (root->left == NULL)
        {
            Node *temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL)
        {
            Node *temp = root->left;
            delete root;
            return temp;
        }

        // If both children exist
        else
        {

            Node *succParent = root;

            // Find successor
            Node *succ = root->right;
            while (succ->left != NULL)
            {
                succParent = succ;
                succ = succ->left;
            }

            // Delete successor.  Since successor
            // is always left child of its parent
            // we can safely make successor's right
            // right child as left of its parent.
            // If there is no succ, then assign
            // succ->right to succParent->right
            if (succParent != root)
                succParent->left = succ->right;
            else
                succParent->right = succ->right;

            // Copy Successor Data to root
            root->data = succ->data;

            // Delete Successor and return root
            delete succ;
            return root;
        }
    }
    bool Delete(unsigned int id)
    {
        Node *temp = deleteNode(root, id);
        if (temp == NULL)
        {
            return false;
        }
        else
        {
            // cout << temp->data.ID_P << endl; // trả về node cha chứa trên node đã bị xóa
            // delete temp; // xóa chỗi này sẽ bị lỗi vì xoa đi mất 1 nút . nút ở trên nút đã bị xóa
            return true;
        }
    }

    void Finding(unsigned int id, int option, bool &modified)
    {
        Node *temp = search(id);

        if (temp == NULL)
        {
            modified = false;
            if (!modified) // điều kiện cần thiết  ko có nó chương trình ko chạy vì nó return; ????
            {
                return;
            }
        }
        else
        {
            switch (option)
            {

            case 1:
            {
                cout << "\t\t\t\t\t\tProduct Code     :       ";
                cin >> temp->data.ID_P;
                cin.ignore();
            }
            break;
            case 2:
            {
                cout << "\t\t\t\t\t\tName of Product  :       ";
                getline(cin, temp->data.NameProduc);
            }
            break;
            case 3:
            {
                cout << "\t\t\t\t\t\tDate of Input    :       ";
                getline(cin, temp->data.Input_P);
            }
            break;
            case 4:
            {
                cout << "\t\t\t\t\t\tMade in          :       ";
                getline(cin, temp->data.MadeIn);
            }
            break;
            case 5:
            {
                cout << "\t\t\t\t\t\tBrand of Product :       ";
                getline(cin, temp->data.Brand);
            }
            break;
            case 6:
            {
                cout << "\t\t\t\t\t\tWeight of Product:       ";
                cin >> temp->data.Weight;
                cin.ignore();
            }
            break;
            case 7:
            {
                cout << "\t\t\t\t\t\tPower of Product :       ";
                cin >> temp->data.Power;
                cin.ignore();
            }
            break;
            case 8:
            {
                cout << "\t\t\t\t\t\tPrice of Product :     ";
                cin >> temp->data.Price;
                cin.ignore();
            }
            break;
            case 9:
            {
                cout << "\t\t\t\t\t\tNumber of Product :     ";
                cin >> temp->data.Number;
                cin.ignore();
            }
            break;
            default:
                cout << "\t\t\t\t\t\t The option is not available !";
            }
        }
        modified = true;
    }

    void PLine(int n, char ch)
    {
        cout << endl;
        for (int i = 0; i < n; i++)
            cout << ch;
        cout << endl;
    }
    void uploading(Node *root)
    {
        if (root != NULL)
        {
            uploading(root->left);
            stack.push_back(root->data);
            uploading(root->right);
        }
    }

    void inorder_traversal(Node *root)
    {
        if (root != NULL)
        {
            inorder_traversal(root->left);
            PLine(108, '-');
            cout << "| " << root->data.ID_P << " | " << root->data.NameProduc << "\t| " << root->data.Input_P << "\t| "
                 << root->data.MadeIn << "\t\t| " << root->data.Brand << "\t\t| " << root->data.Weight << "\t| "
                 << root->data.Power << "\t| " << root->data.Price << "\t| " << root->data.Number << "\t|" << endl;
            inorder_traversal(root->right);
        }
    }
    void show()
    {
        Node *tempNode = root;
        inorder_traversal(tempNode);
    }
};
class Screem
{
private:
    Tree_Mannager_System m;
    Curstomer_LinkedList n;

public:
    /* Screen Admin*/
    Screen()
    {
        m.Loading_Data();
        n.Loading_Data_C();
        n.Loading_Data_PC();
    }
    void searchInfo()
    {
        unsigned int id;
        cout << endl
             << endl
             << endl
             << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\tEnter Product ID number: ";
        cin >> id;
        cin.ignore();
        Product *key = m.Seaching(id);

        if (key != NULL)
        {
            // system("cls");
            cout << endl
                 << endl;
            cout << "\t\t\t\t\t\tProduct informations" << endl;
            cout << "\t\t\t\t\t\tProduct ID number  :  " << key->ID_P << endl;
            cout << "\t\t\t\t\t\tName of Product    :  " << key->NameProduc << endl;
            cout << "\t\t\t\t\t\tDate of birth      :  " << key->Input_P << endl;
            cout << "\t\t\t\t\t\tMade of Product    :  " << key->MadeIn << endl;
            cout << "\t\t\t\t\t\tBrand of Product   :  " << key->Brand << endl;
            cout << "\t\t\t\t\t\tWeight of Product  :  " << key->Weight << endl;
            cout << "\t\t\t\t\t\tPower              :  " << key->Power << endl;
            cout << "\t\t\t\t\t\tPrice of Product   :  " << key->Price << endl;
            cout << "\t\t\t\t\t\tNumber of Product  :  " << key->Number << endl;
            cout << "\t\t\t\t\t\t**********************************************" << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\tSearched successfully!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;

            getchar();
            showMenu();
        }
        else
        {
            cout << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\tNo information to display!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar();
            showMenu();
        }
    }

    void deleteInfo()
    {
        unsigned int id;
        bool deleted = false;
        cout << endl
             << endl
             << endl
             << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\tEnter Product ID number: ";
        cin >> id;
        cin.ignore();
        deleted = m.Delete(id);
        if (deleted)
        {
            cout << "\t\t\t\t\t\tProduct " << id << " number is deleted!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar();
            showMenu();
        }
        else
        {
            cout << "\t\t\t\t\t\tDo not have this Product!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar();
            showMenu();
        }
    }
    void modifyInfo()
    {
        unsigned int id;
        bool modified = false;
        system("cls");
        cout << endl
             << endl
             << endl
             << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\tProduct ID number: ";
        cin >> id;
        cin.ignore();

        while (true)
        {
            int option;
            system("cls");
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\tChoose item that you want to modify?" << endl
                 << endl;
            cout << "\t\t\t\t\t\t**************************************" << endl;
            cout << "\t\t\t\t\t\t*                                    *" << endl;
            cout << "\t\t\t\t\t\t*        1. Product ID number        *" << endl;
            cout << "\t\t\t\t\t\t*        2. Name of Prodcut          *" << endl;
            cout << "\t\t\t\t\t\t*        3. Date of Input            *" << endl;
            cout << "\t\t\t\t\t\t*        4. Made in From             *" << endl;
            cout << "\t\t\t\t\t\t*        5. Brand of Product         *" << endl;
            cout << "\t\t\t\t\t\t*        6. Weight of Product        *" << endl;
            cout << "\t\t\t\t\t\t*        7. Power of Product         *" << endl;
            cout << "\t\t\t\t\t\t*        8. Price of Product         *" << endl;
            cout << "\t\t\t\t\t\t*        9. Number of Product        *" << endl;
            cout << "\t\t\t\t\t\t*                                    *" << endl;
            cout << "\t\t\t\t\t\t**************************************" << endl;
            cout << "\t\t\t\t\t\tPlease enter your choose: ";
            cin >> option;
            cin.ignore();
            m.Finding(id, option, modified);
            if (!modified)
            {
                break;
            }
            char choose;
            cout << "\t\t\t\t\t\tDo you want to continue? (Y/N): ";
            cin >> choose;
            cin.ignore();
            if (choose == 'N' || choose == 'n')
                break;
        }

        if (modified)
        {
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\t Modified successfully!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar();
            showMenu();
        }
        else
        {
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\tDo not found this Product" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar();
            showMenu();
        }
    }
    void enterInfo()
    {
        Product data;
        char choose;
        do
        {
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\tProduct Code     :       ";
            cin >> data.ID_P;
            cin.ignore();
            cout << "\t\t\t\t\t\tName of Product  :       ";
            getline(cin, data.NameProduc);

            cout << "\t\t\t\t\t\tDate of Input    :       ";
            getline(cin, data.Input_P);

            cout << "\t\t\t\t\t\tMade in From     :       ";
            getline(cin, data.MadeIn);

            cout << "\t\t\t\t\t\tBrand of Product :       ";
            getline(cin, data.Brand);

            cout << "\t\t\t\t\t\tWeight of Product:       ";
            cin >> data.Weight;
            cin.ignore();
            cout << "\t\t\t\t\t\tPower of Product :       ";
            cin >> data.Power;
            cin.ignore();
            cout << "\t\t\t\t\t\tPrice of Product :       ";
            cin >> data.Price;
            cin.ignore();
            cout << "\t\t\t\t\t\tNumber of Product:       ";
            cin >> data.Number;
            cin.ignore();
            cout << endl;

            m.inserts(data);

            cout << "\t\t\t\t\t\tDo you want to continue? (Y/N): ";
            cin >> choose;
            cin.ignore();

        } while (choose == 'Y' || choose == 'y');
        showMenu();
        getchar(); // enter any key from keyboard
    }
    void displayInfo()
    {
        cout << "\n============================================ List Of Product ==========================================";
        cout << "\n"
             << "| ID | Name \t| Input_day\t| Made in\t| Brand\t\t\t| Weight | Power | Price | Number |" << endl;
        m.show();
        cout << endl
             << endl
             << endl;
        // print notification
        cout << "\t\t\t\t\t\tDisplayed all informations of Product!" << endl;
        cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
        getchar();
        showMenu(); // go back menu screen
    }

    void showMenu()
    {
        system("cls"); // xóa màng hình ở vscode để cho nó đẹp hơn !
        int option;
        cout << endl
             << endl
             << endl
             << endl
             << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\t***************************************************" << endl;
        cout << "\t\t\t\t\t\t*                                                 *" << endl;
        cout << "\t\t\t\t\t\t*         1. Add Product informations             *" << endl;
        cout << "\t\t\t\t\t\t*         2. Modify Product informations          *" << endl;
        cout << "\t\t\t\t\t\t*         3. Delete Product informations          *" << endl;
        cout << "\t\t\t\t\t\t*         4. Search Product informations          *" << endl;
        cout << "\t\t\t\t\t\t*         5. Display Product informations         *" << endl;
        cout << "\t\t\t\t\t\t*         6. Exit                                 *" << endl;
        cout << "\t\t\t\t\t\t*                                                 *" << endl;
        cout << "\t\t\t\t\t\t***************************************************" << endl;
        cout << "\t\t\t\t\t\tPlease enter your choose: ";
        cin >> option;
        cin.ignore();
        switch (option)
        {
        case 1:
        {
            system("cls");
            enterInfo();
        }
        break;

        case 2:
        {
            system("cls");
            modifyInfo();
        }
        break;

        case 3:
        {
            system("cls");
            deleteInfo();
        }
        break;

        case 4:
        {
            system("cls");
            searchInfo();
        }
        break;

        case 5:
        {
            system("cls");
            displayInfo();
        }
        break;

        case 6:
        {
            Save();
            system("cls");
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\t***********************************************" << endl;
            cout << "\t\t\t\t\t\t*                                             *" << endl;
            cout << "\t\t\t\t\t\t*        You have exited the system!          *" << endl;
            cout << "\t\t\t\t\t\t*                Thank you!!                  *" << endl;
            cout << "\t\t\t\t\t\t*                                             *" << endl;
            cout << "\t\t\t\t\t\t***********************************************" << endl;

            exit(0);
        }
        break;

        default:
        {
            system("cls");
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\t   Sorry! The system do not have this function!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar(); // hàm getchar này để nó ngắc lại đợi người dùng ấn bất kỳ phím gì thì nó mới cho qua
            // mục tiêu là để dừng màng hình lại cho ng dùng dễ quan sát !.
            showMenu();
        }
        }
    }
    void Save()
    {
        char choose;
        system("cls");
        cout << endl
             << endl
             << endl
             << endl
             << endl
             << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\t*********************************************" << endl;
        cout << "\t\t\t\t\t\t*                                           *" << endl;
        cout << "\t\t\t\t\t\t*        Do You Want To Save Modify!        *" << endl;
        cout << "\t\t\t\t\t\t*                                           *" << endl;
        cout << "\t\t\t\t\t\t*********************************************" << endl;
        cout << "\t\t\t\t\t\t Please enter your choose (Y/N): ";
        cin >> choose;
        cin.ignore();

        if (choose == 'Y' || choose == 'y')
        {
            m.Uploading_Data();
        }
    }
    // ********************************************************************************************************
    /*** screen curstomer */
    // ********************************************************************************************************
    // ********************************************************************************************************
    // ********************************************************************************************************
    void checking_bill()
    {

        unsigned int id_c;
        int check = 0;
        system("cls");

        while (true)
        {
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\tCustomer ID number: ";
            cin >> id_c;
            cin.ignore();
            vector<Product> item = n.Get_P_C(id_c, check);
            cout << endl
                 << endl;
            cout << "\t\t\t\t\t\t*******************************************" << endl;
            cout << "\t\t\t\t\t\t*** Name of Product ****** Price *** Number " << endl;
            for (const auto &element : item)
            {
                cout << "\t\t\t\t\t\t" << element.NameProduc << "\t\t" << element.Price << "\t\t" << element.Number << endl;
            }

            cout << "\t\t\t\t\t\t********************************************" << endl;
            char choose;
            cout << "\t\t\t\t\t\tDo you want to continue? (Y/N): ";
            cin >> choose;
            cin.ignore();
            if (choose == 'N' || choose == 'n')
                break;
        }

        if (check)
        {
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\t Checking successfully!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar();
            ShowMenu_c();
        }
        else
        {
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\tDo not found this Customer" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar();
            ShowMenu_c();
            ;
        }
    }

    void Buy_Product()
    {
        int id_p, id_c, Number;
        char choose = 'a';
        do
        {
            cout << "\n============================================ List Of Product ==========================================";
            cout << "\n"
                 << "| ID | Name \t| Input_day\t| Made in\t| Brand\t\t\t| Weight | Power | Price | Number |" << endl;
            m.show();
            cout << endl
                 << endl
                 << endl;
            // print notification
            cout << "\t\t\t\t\t\tDisplayed all informations of Product!" << endl;
            cout << "\t\t\t\t\t\tEnter ID of Product You Want To Buy : ";
            cin >> id_p;
            cin.ignore();
            cout << "\t\t\t\t\t\tHow Many Product Do You Want To Buy : ";
            cin >> Number;
            cin.ignore();
            if (choose != 'Y' && choose != 'y')
            {
                cout << "\t\t\t\t\t\tEnter ID Position of Customer in list : ";
                cin >> id_c;
                cin.ignore();
            }
            Product *key = m.Seaching(id_p);
            key->Number = Number;
            n.Add_P_C(id_c, *key);
            cout << "\t\t\t\t\t\tAdd Product Suseecfull !" << endl;
            cout << "\t\t\t\t\t\tDo you want to continue? (Y/N): ";
            cin >> choose;
            cin.ignore();

        } while (choose == 'Y' || choose == 'y');
        getchar();
        ShowMenu_c();
    }
    void Delete_C()
    {
        unsigned int id;
        bool deleted = false;
        cout << endl
             << endl
             << endl
             << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\tEnter Customer ID number: ";
        cin >> id;
        cin.ignore();
        deleted = n.Remove(id);
        if (deleted)
        {
            cout << "\t\t\t\t\t\tCustomer " << id << " number is deleted!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar();
            ShowMenu_c();
        }
        else
        {
            cout << "\t\t\t\t\t\tDo not have this Product!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar();
            ShowMenu_c();
        }
    }

    void Add_C()
    {
        char choose;
        system("cls");
        do
        {
            Curstomer temp_C;
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\tEnter your information below!!" << endl;
            cout << "\t\t\t\t\t\tCustomer Code         :  ";
            cin >> temp_C.id_cust;
            cin.ignore();
            cout << "\t\t\t\t\t\tName of Product       :  ";
            getline(cin, temp_C.name);
            cout << "\t\t\t\t\t\tGentle of Customer    :  ";
            getline(cin, temp_C.gentle);
            cout << "\t\t\t\t\t\tBirthday of Customer  :  ";
            getline(cin, temp_C.bir_day);
            cout << "\t\t\t\t\t\tNumber of Customer    :  ";
            cin >> temp_C.number;
            cin.ignore();
            cout << endl;
            n.AddValue(temp_C);
            cout << "\t\t\t\t\t\tDo you want to continue? (Y/N): ";
            cin >> choose;
            cin.ignore();
        } while (choose == 'Y' || choose == 'y');
        ShowMenu_c();
        getchar(); // enter any key from keyboard
    }
    void Show_L_C()
    {
        cout << "\n\t=====================================List of Custommer==================================================";
        cout << "\n\t|\t\tID\t\t|\t\tName\t\t|\t\tGender\t\t|\t\tAge\t\t|" << endl;
        n.Show();
        cout << endl
             << endl
             << endl;
        // print notification
        cout << "\t\t\t\t\t\tDisplayed all informations of Customer!" << endl;
        cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
        getchar();
        ShowMenu_c(); // go back menu screen
    }
    void Save_C()
    {
        char choose;
        system("cls");
        cout << endl
             << endl
             << endl
             << endl
             << endl
             << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\t*********************************************" << endl;
        cout << "\t\t\t\t\t\t*                                           *" << endl;
        cout << "\t\t\t\t\t\t*        Do You Want To Save Modify!        *" << endl;
        cout << "\t\t\t\t\t\t*                                           *" << endl;
        cout << "\t\t\t\t\t\t*********************************************" << endl;
        cout << "\t\t\t\t\t\t Please enter your choose (Y/N): ";
        cin >> choose;
        cin.ignore();

        if (choose == 'Y' || choose == 'y')
        {
            n.Uploading_Data_c();
            n.UpLoading_Data_PC();
        }
    }

    void ShowMenu_c()
    {
        system("cls"); // xóa màn hình ở vscode để cho nó đẹp hơn !
        int option;
        cout << endl
             << endl
             << endl
             << endl
             << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\t***************************************************" << endl;
        cout << "\t\t\t\t\t\t*                                                 *" << endl;
        cout << "\t\t\t\t\t\t*         1. Buy Product                          *" << endl;
        cout << "\t\t\t\t\t\t*         2. Add customer                         *" << endl;
        cout << "\t\t\t\t\t\t*         3. Show list customer                   *" << endl;
        cout << "\t\t\t\t\t\t*         4. Delete customer                      *" << endl;
        cout << "\t\t\t\t\t\t*         5. Checking bill of customer            *" << endl;
        cout << "\t\t\t\t\t\t*         6. Exit                                 *" << endl;
        cout << "\t\t\t\t\t\t*                                                 *" << endl;
        cout << "\t\t\t\t\t\t***************************************************" << endl;
        cout << "\t\t\t\t\t\tPlease enter your choose: ";
        cin >> option;
        cin.ignore();
        switch (option)
        {
        case 1:
        {
            system("cls");
            Buy_Product();
        }
        break;
        case 2:
        {
            system("cls");
            Add_C();
        }
        break;
        case 3:
        {
            system("cls");
            Show_L_C();
        }
        break;
        case 4:
        {
            system("cls");
            Delete_C();
        }
        break;
        case 5:
        {
            system("cls");
            checking_bill();
        }
        break;
        case 6:
        {
            Save_C();
            system("cls");
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\t***********************************************" << endl;
            cout << "\t\t\t\t\t\t*                                             *" << endl;
            cout << "\t\t\t\t\t\t*        You have exited the system!          *" << endl;
            cout << "\t\t\t\t\t\t*                Thank you!!                  *" << endl;
            cout << "\t\t\t\t\t\t*                                             *" << endl;
            cout << "\t\t\t\t\t\t***********************************************" << endl;
            exit(0);
        }
        break;
        default:
        {
            system("cls");
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\t   Sorry! The system do not have this function!" << endl;
            cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
            getchar(); // hàm getchar này để nó ngắt lại đợi người dùng ấn bất kỳ phím gì thì nó mới cho qua
            // mục tiêu là để dừng màn hình lại cho ng dùng dễ quan sát !.
            ShowMenu_c();
        }
        }
    }

    /* Screem main */
    void exitSys()
    {

        system("cls");
        cout << endl
             << endl
             << endl
             << endl
             << endl
             << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\t*********************************************" << endl;
        cout << "\t\t\t\t\t\t*                                           *" << endl;
        cout << "\t\t\t\t\t\t*        You have exited the system!        *" << endl;
        cout << "\t\t\t\t\t\t*                                           *" << endl;
        cout << "\t\t\t\t\t\t*********************************************" << endl;
        cout << "\t\t\t\t\t\t   Press enter to exit the program!" << endl;
        getchar();
        exit(0);
    }
    void loginScreen()
    {
        system("cls");
        string userName;
        string password;
        bool check = false;
        int test = 0;
        do
        {
            system("cls");
            cout << endl
                 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            cout << "\t\t\t\t\t\tUsername: ";
            cin >> userName;
            cout << "\t\t\t\t\t\tPassword: ";
            cin >> password;
            cin.ignore();
            if (userName.compare("admin") == 0 && password.compare("admin") == 0)
            {
                cout << endl
                     << endl
                     << endl
                     << endl
                     << endl
                     << endl;
                cout << "\t\t\t\t\t\tAdmin Login successfully!" << endl;
                cout << "\t\t\t\t\t\tPress any key to continue!";
                getchar();
                showMenu();
                check = true;
            }
            else if (userName.compare("staff") == 0 && password.compare("staff") == 0)
            {
                cout << endl
                     << endl
                     << endl
                     << endl
                     << endl
                     << endl;
                cout << "\t\t\t\t\t\tStaff Login successfully!" << endl;
                cout << "\t\t\t\t\t\tPress any key to continue!";
                getchar();
                ShowMenu_c();
                check = true;
            }

            else
            {
                system("cls");
                test++;
                cout << endl
                     << endl
                     << endl
                     << endl
                     << endl
                     << endl;
                cout << "\t\t\t\t\t\tYou are only allowed to try 3 times. You just have " << (3 - test) << " times left!" << endl;
                cout << "\t\t\t\t\t\tPress any key to continue!" << endl;
                getchar();
                if (test == 3)
                {
                    cout << "\t\t\t\t\t\tYou have entered the wrong username or password 3 times and you are not allowed to login to the system!" << endl;
                    getchar();
                    exitSys();
                }
            }
        } while (!check);
    }

    void delay(unsigned int mseconds)
    {
        // Storing start time
        clock_t startTime = clock();
        // looping till required time is not acheived
        while (clock() < startTime + mseconds)
            ;
    }

    void title()
    {
        cout << endl
             << endl
             << endl;
        cout << "\t\t\t\t\t\t********************************************" << endl;
        cout << "\t\t\t\t\t\t*                                          *" << endl;
        cout << "\t\t\t\t\t\t*                                          *" << endl;
        cout << "\t\t\t\t\t\t*        WELL COME TO GREEN MACHINE        *" << endl;
        cout << "\t\t\t\t\t\t*                                          *" << endl;
        cout << "\t\t\t\t\t\t*        PRODUCT MANAGEMENT SYSTEM         *" << endl;
        cout << "\t\t\t\t\t\t*                                          *" << endl;
        cout << "\t\t\t\t\t\t*                                          *" << endl;
        cout << "\t\t\t\t\t\t********************************************" << endl;
    }
    void mainScreen()
    {
        system("cls");
        title();
        cout << endl
             << "\t\t\t\t\t\t\tPress any key to continue!" << endl
             << endl;
        getchar();
        cout << "\t\t\t\t\t\tLoading";
        for (int process = 0; process < 36; process++)
        {
            delay(50); // delay 50 milisecond
            cout << ".";
        }
    }
};
void deploy()
{
    Screem screem;
    screem.mainScreen();
    screem.loginScreen();
}

int main()
{

    deploy();
    return 0;
}
