#include<iostream>
#include<vector>
#include<stack>
#include<string>
using namespace std;



// Heap implementation class.
class MaxHeap {

private:
	int k; // store key
	int num_items; // store number of items
	vector<vector<int>> maxheap; // store maxheap

	
	// sort function put nlocth element back to proper position of node in maxheap
	void sort(int idx, int k, int nloc) {
		if (nloc > 0 && maxheap[idx][nloc] < maxheap[idx][nloc - 1]) {
			for (int i = nloc; i > 0; --i) {
				if (maxheap[idx][i] >= maxheap[idx][i - 1]) {
					break;
				}
				swap(maxheap[idx][i], maxheap[idx][i - 1]);
			}
		} else {
			for (int i = nloc; i < k - 1; ++i) {
				if (maxheap[idx][i] <= maxheap[idx][i + 1]) {
                                        break;
                                }
                                swap(maxheap[idx][i], maxheap[idx][i + 1]);
                        }
		}
	}


	// fixup performs fixup for k-element maxheap
	void fixup(int nloc) {
		int idx = (num_items - 1) / k; // node index
		int loc = (num_items - 1) % k; // item index in node
		sort(idx, loc + 1, nloc); // sort last node
		int pidx = (idx - 1)/2; // parent index
		while (idx != 0 && maxheap[pidx][0] < maxheap[idx][loc]) {
			swap(maxheap[pidx][0], maxheap[idx][loc]); // swap parent first with node last
			sort(pidx, k, 0); // sort parent
			sort(idx, loc + 1, loc); // sort node
			loc = k - 1; // updata item index
			idx = pidx; // update node index
			pidx = (pidx - 1)/2; // update parent index
		}
	}
	
	// fixdown performs fixdown for k-element maxheap starting at node with index n
	void fixdown(int n, int nloc) {
		int idx = n; // node index
		int loc = 0; // item index in node
		int size = maxheap[idx].size();
		sort(idx, size, nloc); // sort first node
		int cidx = 2 * idx + 1; // children index
		while (cidx <= (num_items - 1) / k) {
			int cloc = maxheap[cidx].size() - 1; // right children index
			if (cidx * k + 1 <= (num_items - 1) / k) {
				int crloc = maxheap[cidx + 1].size() - 1; // right children item index
				if (maxheap[cidx + 1][crloc] > maxheap[cidx][cloc]) {
					cidx = cidx + 1; // update children index
					cloc = crloc; // update children item index
				}
			}
			if (maxheap[idx][loc] >= maxheap[cidx][cloc]) {
				break; // already in maxheap form
			}
			swap(maxheap[idx][loc], maxheap[cidx][cloc]); // swap node first with children last
			sort(idx, k, k - 1); // sort node
			sort(cidx, cloc + 1, 0); // sort children
			idx = cidx; // update node index
			cidx = 2 * cidx + 1; // update children index
		}
	}

	void deleteval(int nidx, int nloc)
     {
             int idx = (num_items - 1) / k; // last node index
             int loc = (num_items - 1) % k; // item index of last node
             int dloc = k - 1; // last item index for first node (max value)
             if (idx == 0) {
                     dloc = loc; // node might have less than k element
             }
             maxheap[nidx][nloc] = maxheap[idx][loc]; // replace value with last element
             maxheap[idx].pop_back(); // remove last element from last vector
             if (maxheap[idx].empty()) {
                     maxheap.pop_back(); // remove last vector if empty
             }
             num_items--; // update number of items
	     int pidx = (nidx - 1) / 2;
	     if (nidx > 0 && maxheap[nidx][nloc] > maxheap[pidx][0]) { // item replaced is larger than parent
		     fixup(nloc);
	     }
             fixdown(nidx, nloc); // perform fixdown starting at node nidx
     }
	
	
	// findfifval updates node index(idx) and index(loc) for 5th largest value in maxheap when k = 1, 2, 3, 4
	void findfifval(int& idx, int& loc) {
		int s = maxheap.size(); // number of nodes
		int lidx = 1; // default left node index
		int lmax = maxheap[lidx].size() - 1; // location of max item in that node
		if (s < 3) { // no right child for root node
			idx = lidx;
			loc = lmax;
			if (k == 3) loc--;
		} else { // at least right child exists
			int ridx = 2;  // default right node index for k = 2, 3, 4
			int rmax = maxheap[ridx].size() - 1; // location of max item in that node
			if (k == 4) { // k = 4 case
				if (maxheap[lidx][lmax] > maxheap[ridx][rmax]) {
					idx = lidx;
					loc = lmax;
				} else {
					idx = ridx;
					loc = rmax;
				}
			} else if (k == 2) { // k = 2 case
				int max2 = 0;
				int max1 = 1;
				int idx2 = 1;
				int idx1 = 1;
				if (maxheap[2][rmax] < maxheap[idx1][max1]) {
					if  (maxheap[2][rmax] < maxheap[idx2][max2]) {
						max1 = max2;
						idx1 = idx2;
						max2 = rmax;
                                        	idx2 = 2;
					} else {
						max1 = rmax;
						idx1 = 2;
					}
				}
				if (rmax != 0 && maxheap[2][0] > maxheap[idx1][max1]) {
					max2 = max1;
                                        idx2 = idx1;
                                        max1 = 0;
                                        idx1 = 2;
				} else if (rmax != 0 && maxheap[2][0] > maxheap[idx2][max2]) {
                                        max2 = 0;
                                        idx2 = 2;
				}
				int maxidx = (num_items - 1) / k;
				for (int i = 3; i <= maxidx; ++i) {
					int size = maxheap[i].size();
					for (int j = 0; j < size; ++j) {
						if (maxheap[i][j] > maxheap[idx1][max1]) {
							max2 = max1;
                                        		idx2 = idx1;
                                        		max1 = j;
                                        		idx1 = i;
						} else if (maxheap[i][j] > maxheap[idx2][max2]) {
							max2 = j;
                                                        idx2 = i;
						}
					}

				}
				idx = idx2;
                                loc = max2;
			} else {
				int max1 = lmax; // default index of max1(first max) and max2(second max)
				int max2 = lmax;
				int idx1 = lidx;
				int idx2 = lidx;
				if (k == 3) { // k = 3 case
					max2 = lmax - 1;
					for (int i = rmax; i >= 0; --i) {
						if (maxheap[ridx][i] > maxheap[idx1][max1]) {
							max2 = max1;
                                                        idx2 = idx1;
                                                        max1 = i;
                                                        idx1 = ridx;
                                                } else if (maxheap[ridx][i] > maxheap[idx2][max2]) {
                                                        max2 = i;
                                                        idx2 = ridx;
                                                }
					}
					idx = idx2;
					loc = max2;
				} else { // k = 1 case
					int idx2 = 0;
					int idx3 = 0;
					int idx4 = 0;
					if (maxheap[2][0] > maxheap[idx1][0]) {
						idx2 = idx1;
						idx1 = 2;
					} else {
                        	        	idx2 = 2;
					}
					if (maxheap[3][0] > maxheap[idx1][0]) {
                                                idx3 = idx2;
						idx2 = idx1;
                                                idx1 = 3;
                                        } else if (maxheap[3][0] > maxheap[idx2][0]) {
                                                idx3 = idx2;
						idx2 = 3;
                                        } else {
						idx3 = 3;

					}
					if (maxheap[4][0] > maxheap[idx1][0]) {
                                                        idx4 = idx3;
                                                        idx3 = idx2;
                                                        idx2 = idx1;
                                                        idx1 = 4;
                                        } else if (maxheap[4][0] > maxheap[idx2][0]) {
                                                        idx4 = idx3;
                                                        idx3 = idx2;
                                                        idx2 = 4;
                                        } else if (maxheap[4][0] > maxheap[idx3][0]) {
                                                        idx4 = idx3;
                                                        idx3 = 4;
                                        } else {
                                                        idx4 = 4;
					}
					for (int i = 5; i < num_items && i < 32; ++i) {
						if (maxheap[i][0] > maxheap[idx1][0]) {
							idx4 = idx3;
                                                        idx3 = idx2;
                                                        idx2 = idx1;
							idx1 = i;
						} else if (maxheap[i][0] > maxheap[idx2][0]) {
							idx4 = idx3;
                                                        idx3 = idx2;
                                                        idx2 = i;
						} else if (maxheap[i][0] > maxheap[idx3][0]) {
							idx4 = idx3;
                                                        idx3 = i;
						} else if (maxheap[i][0] > maxheap[idx4][0]) {
							idx4 = i;
						}
					}	
					idx = idx4; // return fifth largest
					loc = lmax;
				}
			}
		}
	}



 public:
     CS240MaxHeap(const int k)
         : k(k), num_items(0) {};
     
     // see the assignment description for specification of what these
     // functions do
     void insert(int a) 
     {
	     int loc = num_items % k; // item index for a
	     if (loc == 0) {
		     vector<int> v; // create a new node and store a
		     v.push_back(a);
		     maxheap.push_back(v);
	     } else {
		     int idx = num_items / k;
		     maxheap[idx].push_back(a); // put a at the end of last node
	     }
	     num_items++; // update number of items
	     fixup(loc); // perform fixup
     }
     int delete_max() 
     {
	     int dloc = k - 1; // find index of last value
	     if (num_items < k) {
                     dloc = num_items - 1; // node might have less than k element
             }
	     int val = maxheap[0][dloc]; // max value
	     deleteval(0, dloc); // perform delete value at last item in the first node
	     return val; // give deleted max value
     }
     int delete_max_5() 
     {
	     int idx = 0; // node
             int loc = 0; // index
	     int val = 0; // value
	     vector<int> v;
	     if (k >= 5) { // case when key => 5
		     loc = maxheap[0].size() - 5; // update index
	     } else { // case when key < 5
		     findfifval(idx, loc);  // find max 5 and store its node and index
	     }
	     val = maxheap[idx][loc];	// store max 5
	     deleteval(idx, loc); // delete max 5 and update maxheap
	     return val;
     }
     void print()
      {
	      for (int i = 0; i < maxheap.size(); ++i) { // go through each node 
		      for(int j = 0; j < maxheap[i].size(); ++j) { // go through each element in a node
			      cout << maxheap[i][j] << " ";
		      }
		      cout << endl;
	      }
      }
      void printLeftPath()
      {
	      for (int i = 0; i < num_items / k; i = 2 * i + 1) { // find left children
		       for(auto j: maxheap[i]) { // go through elements in node
                              cout << j << ' ';
                      }
                      cout << endl;
	      }
      }
      void printRightPath()
      {
	      for (int i = 0; i < num_items / k; i = 2 * i + 2) { // find right children
                       for(auto j: maxheap[i]) { // go through elements in each node
                              cout << j << ' ';
                      }
                      cout << endl;
              }
      }
      int crazy_clean()
      {
	      while (num_items > 1) { // loop until only one item left
		      int a = delete_max(); // delete and store max1
		      int b = delete_max(); // delete and store max2
		      insert(a - b); // insert the difference between a and b
	      }
	      return maxheap[0][0]; // first element
      }
      int size()
      {
	      return num_items;
      }

    };
   

int main() {
    CS240MaxHeap* pq = nullptr;
    int k;
    int total_items = 0;


    while (true) {
        std::string command;
        std::cin >> command;

        if (command == "r") {  // initializes heap with the max_capacity read from the input
            std::cin >> k;

            pq = new CS240MaxHeap(k);
        }
        else if (command == "x") {  // finishes the program and cleans up
            delete pq;
            return 0;
        }
        else if (command == "i") { // inserts item read from std
            int item;
            std::cin >> item;
            pq->insert(item);
            total_items++;
        }
        else if (command == "d") {  // deletes and prints item read from std
            if (pq->size() != 0) {
                total_items--;
                std::cout << pq->delete_max() << std::endl;
            }
            else {
                std::cout << "Empty heap!" << std::endl;
            }
        }
        else if (command == "d_") {  // deletes and doesn't not print item read from std
            if (pq->size() != 0) {
                total_items--;
                pq->delete_max();
            }
            else {
                std::cout << "Empty heap!" << std::endl;
            }
        }
        else if (command == "d5") {  // deletes and prints 5th largest item
            if (pq->size() >= 5) {
                total_items--;
                cout << pq->delete_max_5() << endl;
            }
        	    else {
                std::cout << "Heap has less than 5 elements" << std::endl;
            }
        }
        else if (command == "d_5") {  // deletes and does not print 5th largest item
            if (pq->size() >= 5) {
                total_items--;
                pq->delete_max_5();
            }
            else {
                std::cout << "Heap has less than 5 elements" << std::endl;
            }
        }
        else if (command == "pa") { // prints the heap, including total number of items and k
            if (pq->size() != 0) {
                cout << total_items << endl;
                cout << k << endl;
                pq->print();
            }
        }
        else if (command == "pl") {  // prints the left path, including total number of items and k
            if (pq->size() != 0) {
                cout << total_items << endl;
                cout << k << endl;
                pq->printLeftPath();
            }
        }
        else if (command == "pr") {  // prints the right path,  including total number of items and k
            if (pq->size() != 0) {
                cout << total_items << endl;
                cout << k << endl;
                pq->printRightPath();
            }
        }
        else if (command == "es") {
            int sum = pq->crazy_clean();
            std::cout << sum << endl;
        }
    }
}
