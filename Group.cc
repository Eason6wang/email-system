#include<iostream>
#include<string>
#include<sstream>
#include"Group.h"

using namespace std;

//..................... Group..........................

// indentation gives n indents to ss
void Group::indentation(stringstream &ss, int n) const{
  while(n){
   ss << "\t";
   --n;
  }
}

string Group::name() const{
  return gname;
}

string Group::emails() const{
  stringstream ss;
  // output the group name and emails
  indentation(ss, indent);
  ss << name();
  ss << "\n";
  indentation(ss, indent);
  EmailNode *emailnode = enode;
  ss << "[";
  while(emailnode){
    ss << *emailnode;
    if(emailnode->next != nullptr){
      ss << ", ";
    }
    emailnode = emailnode->next;
  }
  ss << "]\n";
  //output the subgroups info
  GroupNode *groupnode = gnode;
  int tmp = indent;
  indent = 0;
  while(groupnode){
    groupnode->group()->indent = tmp + 1;
    ss << *groupnode;
    groupnode = groupnode->next;
  }
  groupnode = nullptr;
  emailnode = nullptr;
  string result = ss.str();
  return result;
}


//default ctor, name do not have default value
Group::Group(const string &name ):gname{name},
                                  gnode{nullptr},
                                  enode{nullptr},
                                  indent{0}
                                  {}


Group::~Group(){
  delete gnode;
  delete enode;
}

void Group::addGroup(Group *newgroup){
  //base case
  if(gnode == nullptr){
    gnode = new GroupNode{newgroup};
    return;
  }
  GroupNode *newg = new GroupNode{newgroup};
  gnode->addGroup(newg);
}


void Group::addAddress(const string &newaddress){
  if(enode == nullptr){
    enode = new EmailNode{newaddress};
    return;
  }
  enode->addAddress(newaddress);
}

bool Group::removeGroup(const string &groupname){
  if(gnode == nullptr){
    return false;
  }
  // check if the first node should be removed
  if(gnode->group()->name() == groupname){
    GroupNode *tmp = gnode;
    gnode = gnode->next;
    tmp->next = nullptr;
    delete tmp;
    //debug
    cout << "here" << endl;
    return true;
  }
  return gnode->removeGroup(groupname);
}

bool Group::removeAddress(const string &rmaddress){
  if(enode == nullptr && gnode == nullptr){
    return false;
  }
  if(enode == nullptr && gnode != nullptr){
    return gnode->removeAddress(rmaddress);
  }
  // check if the first one can be removed
  if(enode->address() == rmaddress){
    EmailNode *tmp = enode;
    enode = enode->next;
    tmp->next = nullptr;
    delete tmp;
    return true;
  }
  // if the first cannot be removed
  if(enode->removeAddress(rmaddress)){
    return true;
  } else if(gnode == nullptr) {
    return false;
  } else {
    return gnode->removeAddress(rmaddress);
  }
}

Group::GroupNode *Group::findGroup(const string &groupname){
  if(gnode == nullptr){
    return nullptr;
  }
  return gnode->findGroup(groupname);
}

Group::EmailNode *Group::findAddress(const string &addr){
  if(enode){
    EmailNode *result1 = enode->findAddress(addr);
    if(result1 != nullptr){
      return result1;
    }
  }
  if(gnode){
    GroupNode *result2 = gnode->findAddress(addr);
    if(result2 == nullptr){
      return nullptr;
    }
    return result2->group()->findAddress(addr);
  }
  return nullptr;
}

//....................EmailNode...........................

string Group::EmailNode::address() const{
  return emailaddr;
}

Group::EmailNode::EmailNode(const string &address):
 emailaddr{address}, next{nullptr}{}


Group::EmailNode::~EmailNode(){
  delete next;
}

void Group::EmailNode::addAddress(const string &newaddr){
  EmailNode *newnode = new EmailNode{newaddr};
  if(address() > newaddr){
    newnode->next = this->next;
    this->next = newnode;
    string tmp = this->emailaddr;
    this->emailaddr = next->emailaddr;
    next->emailaddr = tmp;
    return;
  }
  if(next == nullptr){
    next = newnode;
  } else {
    next->addAddress(newaddr);
  }
}



bool Group::EmailNode::removeAddress(const string &addr){
  // we start from the second node
  if(next == nullptr){
    return false;
  }
  if(next->address() == addr){
    EmailNode *tmp = next;
    next = next->next;
    tmp->next = nullptr;
    delete tmp;
    return true;
  }
  return next->removeAddress(addr);
}

Group::EmailNode * Group::EmailNode::findAddress(const string &addr){
  if(this->address() == addr){
    return this;
  }
  if(next == nullptr){
    return nullptr;
  }
  return next->findAddress(addr);
}



//.................... GroupNode...........................

Group *Group::GroupNode::group() const{
  return curgroup;
}

Group::GroupNode::GroupNode(Group *newgroup):
 curgroup{newgroup}, next{nullptr}{}


Group::GroupNode::~GroupNode(){
  delete curgroup;
  delete next;
}


void Group::GroupNode::addGroup(GroupNode *gnode){
  if(this->group()->name() > gnode->group()->name()){
    gnode->next = next;
    this->next = gnode;
    Group *tmp = curgroup;
    curgroup = gnode->curgroup;
    gnode->curgroup = tmp;
    tmp = nullptr;
    return;
   }
   if(this->next == nullptr){
     next = gnode;
   } else {
     return next->addGroup(gnode);
  }
}


// why do we need this
/*bool Group::GroupNode::removeGroup(GroupNode *gnode){
  delete gnode->curgroup;
  gnode->curgroup = nullptr;
  gnode->next = nullptr;
  delete gnode;

}
*/

bool Group::GroupNode::removeAddress(const string &addr){
  //check the subgroup one by one
  if(group()->removeAddress(addr)){
    return true;
  }
  if(next == nullptr){
    return false;
  } else {
    return next->removeAddress(addr);
  }
}



bool Group::GroupNode::removeGroup(const string &groupname){
  if(this->group()->removeGroup(groupname)){
    return true;
  }
  // we start from the second node
  if(next == nullptr){
    return false;
  }
  if(next->group()->name() == groupname){
    GroupNode *tmp = next;
    next = next->next;
    delete(tmp->curgroup);
    tmp->curgroup = nullptr;
    tmp->next = nullptr;
    delete(tmp);
    return true;
  }
  return next->removeGroup(groupname);
}


Group::GroupNode *Group::GroupNode::findGroup(const string &groupname){
  if(group()->name() == groupname){
    return this;
  }
  //check if the subgroups contain the name
  GroupNode * result = group()->findGroup(groupname);
  if(result != nullptr){
    return result;
  }
  if(next == nullptr){
    return nullptr;
  }
  return next->findGroup(groupname);
}

Group::GroupNode *Group::GroupNode::findAddress(const string &addr){
  EmailNode *result = group()->findAddress(addr);
  if(result){
    return this;
  }
  if(next){
    return next->findAddress(addr);
  }
  return nullptr;
}



ostream & operator<<(ostream &out, Group & curgroup){
  out << curgroup.emails();
  return out;
}

ostream & operator<<(ostream &out, Group::EmailNode & enode){
  out << enode.address();
  return out;
}

ostream & operator<<(ostream &out, Group::GroupNode & gnode){
  out << *gnode.group();
  return out;
}
