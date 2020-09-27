#include <iostream>
#include <queue>
#include <stack>
 
// The idea of the solution is simple: i use two queues and one stack right
// in the middle of them. So when we want to add a common member of
// the queue we add him either to the first or to the second queue
// depending on whether the queue is empty or not, also we can move
// the member of the second queue to the first queue depending on 
// whether the first queue contains less than a half of the number of
// members. And the main part - adding priveleged member (PM): so basically 
// we add new PM we can add him either to the top of the stack or to the end
// of the first queue - it depends on whether the first queue contains 
// at least half of members or not. Overall, it is the whole algorithm - you
// can always talk to me personally for further details.


int answer()
{
    // i think that roles of the further variables are obvious
	int number_of_queries;
	std::cin >> number_of_queries;
	int current_members_number = 0;
    // head is the first queue
	std::queue<int> head;
    // tale is the second queue
	std::queue<int> tale;
    // middle is the stack between two queues
	std::stack<int> middle;
	for (int query = 0; query < number_of_queries; query++)
	{
        // symb stands for symbol before the member number: "+" or "-"
		char symb;
        // number is member number
		int number;
		std::cin >> symb;
        // because we don't have member number after "-"
		if (symb != '-')
			 std::cin>>number;
		if (symb == '+')
		{
			tale.push(number);
            // as i already mentioned, we can move members from the second
            // queue and middle stack to the first queue. Although, it is 
            // useless and even wrong in case we have an odd number of members
			if (current_members_number % 2 == 0)
			{
                // we move member from the second queue only if all the PM in front of them
                // have gone to the first queue
				if (middle.empty())
				{
					head.push(tale.front());
					tale.pop();
				}
                // if not all the PM have gone then we move them
				else
				{
					head.push(middle.top());
					middle.pop();
				}
			}
            // updating members number variable since we have added one new member
			current_members_number++;
		}
		else if (symb == '*')
		{
            // it makes sense to add new PM to the end of
            // the first queue only if we are sure that no one
            // will try to take place in front of him. 
            // if the number of members is odd, we can't guarantee that.
            // we add the new member to the middle stack in this case
			if (current_members_number % 2 == 0)
				head.push(number);
			else
				middle.push(number);
            // updating members number variable since we have added one new member
			current_members_number++;
		}
        // if (symb == "-"), in other words
		else
		{
            // updating all the structures basing on ideas, written above
			if (current_members_number % 2 == 0)
			{
				if (!middle.empty())
				{
					head.push(middle.top());
					middle.pop();
				}
				else if (!tale.empty())
				{
					head.push(tale.front());
					tale.pop();
				}
			}
            // updating members number variable since we have removed one member
			current_members_number--;
			std::cout << head.front() << std::endl;
			head.pop();
		}
	}
 
	return 0;
}
 

int main()
{
	answer();
}
