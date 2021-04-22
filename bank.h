//
//  bank.h
//
//

#ifndef __BANK_H__
#define __BANK_H__

#include "customer.h"


class Bank {
public:
  Bank() = default;
  Bank(const ext_vector<int>& available) : avail(available), customers() { }
  
  ext_vector<int> get_avail() const { return avail; }
  bool is_avail(const ext_vector<int>& req) const { return req < avail; }
  
  bool is_safe(int id, const ext_vector<int>& req) 
  { 
      bool approve = false;
      ext_vector<int> copyResource = this->avail;
      copyResource -= req; //force an approval 

      // go through the resources to find the availability
      for (size_t i = 0; i < this->avail.size(); ++i) {
          if (i == id) continue; // don't count the forced approval - need to find a method that doesn't use continue

          // if the resources are free, create the request.
          if (copyResource >= this->customers[x]->create_req()) {
              approve = true;
          }
      }

      // we set approve to true in the if statement above
      return approve; 
  } 

  bool req_approved(int id, const ext_vector<int>& req) {
    if (req > avail) { return false; }

    const Customer* c = customers[id];
    if (c->needs_exceeded(req)) { return false; }

    return is_safe(id, req);
  }
  
  void add_customer(Customer* c) {
    customers.push_back(c);
    avail -= c->allocated();
  }
  
  void withdraw_resources(const ext_vector<int>& req) {
    if (!is_avail(req)) {
      pthread_mutex_lock(&mutex_);
      std::cerr << "WARNING: req: " << req << " is not available for withdrawing\n";
      pthread_mutex_unlock(&mutex_);
      return;
    }
    if (is_avail(req)) { avail -= req; }
  }
  void deposit_resources(const ext_vector<int>& req) { avail += req; }


  ext_vector<Customer*> get_customers() const { return customers; }
  
  void show() const {
    pthread_mutex_lock(&mutex_);
    std::cout << "avail: [" << avail << "]\n";
    pthread_mutex_unlock(&mutex_);
    
    for (Customer* c : customers) {
      c->show();
    }
    std::cout << "\n";
  }
  
  friend std::ostream& operator<<(std::ostream& os, const Bank& be) {
    be.show();
    return os;
  }

private:
  ext_vector<int> avail;
  ext_vector<Customer*> customers;
};

#endif /* Bank_h */
