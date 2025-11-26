#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// ================================================================
//                       SONG NODE STRUCT
// ================================================================
struct SongNode {
    string title;
    string artist;
    int duration;
    SongNode* next;
    SongNode* prev;

    SongNode(string t, string a, int d) {
        title = t;
        artist = a;
        duration = d;
        next = prev = NULL;
    }
};

// ================================================================
//             CIRCULAR DOUBLY LINKED LIST CLASS
// ================================================================
class CircularDoublyLinkedList {
public:
    SongNode* head;
    SongNode* current;
    int size;

    CircularDoublyLinkedList() {
        head = NULL;
        current = NULL;
        size = 0;
    }

    // -------------------- ADD SONG --------------------
    void add(string title, string artist, int duration) {
        SongNode* newNode = new SongNode(title, artist, duration);

        if (!head) { // first node
            head = newNode;
            newNode->next = newNode->prev = newNode;
            current = newNode;
        }
        else {
            SongNode* tail = head->prev;
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }

        size++;
    }

    // -------------------- DELETE SONG --------------------
    bool remove(string title) {
        if (!head) return false;

        SongNode* node = head;

        do {
            if (node->title == title) {

                if (size == 1) { // only 1 node
                    delete node;
                    head = current = NULL;
                }
                else {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;

                    if (node == head)
                        head = node->next;
                    if (node == current)
                        current = node->next;

                    delete node;
                }

                size--;
                return true;
            }

            node = node->next;

        } while (node != head);

        return false;
    }

    // -------------------- NAVIGATION --------------------
    void nextSong() {
        if (current) current = current->next;
    }

    void prevSong() {
        if (current) current = current->prev;
    }

    void rewind() {
        current = head;
    }

    // ================================================================
    //              Convert to Vector (for sorting)
    // ================================================================
    vector<SongNode*> toVector() {
        vector<SongNode*> arr;
        if (!head) return arr;

        SongNode* node = head;

        do {
            arr.push_back(node);
            node = node->next;
        } while (node != head);

        return arr;
    }

    // ================================================================
    //                      LINEAR SEARCH (DSA)
    // ================================================================
    SongNode* linearSearch(string key) {
        if (!head) return NULL;

        // convert key to lowercase
        for (char &c : key) c = tolower(c);

        SongNode* node = head;

        do {
            string t = node->title;
            string a = node->artist;

            for (char &c : t) c = tolower(c);
            for (char &c : a) c = tolower(c);

            if (t == key || a == key)
                return node;

            node = node->next;

        } while (node != head);

        return NULL; 
    }

    // ================================================================
    //                      BUBBLE SORT
    // ================================================================
    void bubbleSort(int type) {
        if (size <= 1) return;

        vector<SongNode*> arr = toVector();

        for (int i = 0; i < arr.size() - 1; i++) {
            for (int j = 0; j < arr.size() - i - 1; j++) {

                bool swapNeeded = false;

                if (type == 1) {  
                    if (arr[j]->title > arr[j + 1]->title)
                        swapNeeded = true;
                }
                else if (type == 2) { 
                    if (arr[j]->duration > arr[j + 1]->duration)
                        swapNeeded = true;
                }

                if (swapNeeded)
                    swap(arr[j], arr[j + 1]);
            }
        }

        // rebuild CDLL
        head = NULL;
        current = NULL;
        size = 0;

        for (auto node : arr) {
            node->next = node->prev = NULL;
            add(node->title, node->artist, node->duration);
        }
    }

    // ================================================================
    //                   PRINT PLAYLIST
    // ================================================================
    void printPlaylist() {
        if (!head) {
            cout << "\nPlaylist is empty.\n";
            return;
        }

        cout << "\n--- Playlist (Circular Doubly Linked List) ---\n";

        SongNode* node = head;

        do {
            cout << node->title << " - " << node->artist 
                 << " (" << node->duration << " sec)";

            if (node == current) 
                cout << "   <-- CURRENT";

            cout << endl;

            node = node->next;

        } while (node != head);
    }
};

// ================================================================
//                 RECENTLY PLAYED QUEUE (SIZE 4)
// ================================================================
class RecentlyPlayedQueue {
    SongNode* arr[4];
    int front, rear, count;

public:
    RecentlyPlayedQueue() {
        front = 0;
        rear = -1;
        count = 0;
    }

    void enqueue(SongNode* s) {
        if (count == 4) {
            front = (front + 1) % 4;
            count--;
        }

        rear = (rear + 1) % 4;
        arr[rear] = s;
        count++;
    }

    void print() {
        cout << "\n--- Recently Played (Queue) ---\n";

        if (count == 0) {
            cout << "No recently played songs.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            int idx = (front + i) % 4;
            cout << arr[idx]->title << " - " << arr[idx]->artist << endl;
        }
    }
};

// ================================================================
//                     MENU-DRIVEN MAIN PROGRAM
// ================================================================

int main() {
    CircularDoublyLinkedList playlist;
    RecentlyPlayedQueue recent;

    int choice;

    while (true) {
        cout << "\n\n================ PLAYLIST MENU ================\n";
        cout << "1.  Add Song\n";
        cout << "2.  Delete Song\n";
        cout << "3.  Next Song\n";
        cout << "4.  Previous Song\n";
        cout << "5.  Rewind (Go to First)\n";
        cout << "6.  Linear Search Song\n";
        cout << "7.  Sort Playlist\n";
        cout << "8.  Show Playlist\n";
        cout << "9.  Show Recently Played\n";
        cout << "10. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string title, artist;
            int duration;

            cin.ignore();
            cout << "Enter title: ";
            getline(cin, title);

            cout << "Enter artist: ";
            getline(cin, artist);

            cout << "Duration (sec): ";
            cin >> duration;

            playlist.add(title, artist, duration);
        }

        else if (choice == 2) {
            string title;
            cin.ignore();
            cout << "Enter title to delete: ";
            getline(cin, title);

            if (playlist.remove(title))
                cout << "Song deleted!\n";
            else
                cout << "Song not found!\n";
        }

        else if (choice == 3) {
            if (playlist.current)
                recent.enqueue(playlist.current);

            playlist.nextSong();
        }

        else if (choice == 4) {
            playlist.prevSong();
        }

        else if (choice == 5) {
            playlist.rewind();
        }

        else if (choice == 6) {
            string key;
            cin.ignore();
            cout << "Enter exact Title or Artist to search: ";
            getline(cin, key);

            SongNode* res = playlist.linearSearch(key);

            if (res) {
                cout << "\nSong Found:\n";
                cout << res->title << " - " << res->artist 
                     << " (" << res->duration << " sec)\n";
            }
            else {
                cout << "\nSong NOT found.\n";
            }
        }

        else if (choice == 7) {
            int type;
            cout << "Sort by:\n1. Title (A-Z)\n2. Duration (Short→Long)\nChoice: ";
            cin >> type;

            playlist.bubbleSort(type);
            cout << "Sorting complete!\n";
        }

        else if (choice == 8) {
            playlist.printPlaylist();
        }

        else if (choice == 9) {
            recent.print();
        }

        else if (choice == 10) {
            break;
        }

        else {
            cout << "Invalid Option!\n";
        }
    }

    return 0;
}
