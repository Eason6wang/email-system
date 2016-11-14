#ifndef GROUP_H
#define GROUP_H

#include <string>

class Group {
public:
        Group( const std::string & name );
        ~Group();
        std::string name() const;    // return the group's name
        std::string emails() const;  // return a string containing all of the email addresses in the group
        void addGroup( Group * );   // add the group as a subgroup to this one
        void addAddress( const std::string & ); // add the given email address to the group
        bool removeGroup( const std::string & ); // remove the first occurrence of the subgroup with that name
        bool removeAddress( const std::string & ); // remove the first occurrence of the address

        class EmailNode {
        public:
                EmailNode( const std::string &  );
                ~EmailNode();
                void addAddress( const std::string & );
                bool removeAddress( const std::string & );
                std::string address() const;
                EmailNode * findAddress( const std::string & );
        private:
        // add private information
                friend class Group;
                std::string emailaddr;
                EmailNode *next;
        };

        class GroupNode {
        public:
                GroupNode( Group * );
                ~GroupNode();
                Group * group() const;

                void addGroup( GroupNode* ); // add to the subgroup, in lexicographic order
                bool removeGroup( GroupNode* ); // remove from the linked list

                // Remove the first occurrence of the specified email address from the group
                bool removeAddress( const std::string & );

                // Remove the first occurrence of the group whose name is the specified string
                bool removeGroup( const std::string & );

                // Find the node holding the group with the specified name, or nullptr
                GroupNode * findGroup( const std::string & );

                // Find the node holding the group that has the specified address, or nullptr
                GroupNode * findAddress( const std::string & );
        private:
        // add private information
                Group *curgroup;
                GroupNode *next;

                friend class Group;
        };

        // Return the address of the first node containing the subgroup
        // with the specified name or nullptr if not found
        GroupNode * findGroup( const std::string & );

        // Return the address of the first node containing the subgroup
        // with the specified email address or nullptr if not found
        EmailNode * findAddress( const std::string & );

private:
        std::string gname;
        mutable int indent;
        GroupNode *gnode;
        EmailNode *enode;
        void indentation(std::stringstream &, int) const;
    // add private information
};

std::ostream & operator<<( std::ostream &, Group & );
std::ostream & operator<<(std::ostream &, Group::EmailNode & );
std::ostream & operator<<(std::ostream &, Group::GroupNode & );


#endif
