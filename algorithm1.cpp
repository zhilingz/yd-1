#include<iostream>
#include<vector>
using namespace std;
// 738.单调递增的数字
// 给定一个非负整数 N，找出小于或等于 N 的最大的整数，同时这个整数需要满足其各个位数上的数字是单调递增。

// （当且仅当每个相邻位数上的数字 x 和 y 满足 x <= y 时，我们称这个整数是单调递增的。）
// 先将数字的每个位分解出来，可以使用自定义函数，也可以使用to_string函数，直接转为字符串，然后从前向后比较相邻两个数字之间的大小，发现前一个比后一个小时直接退出，并按照当前元素减一，但是这里需要从后往前去递推，防止减一后破坏升序结构，后面的元素全部置为9的原则，最终得到对应的数字序列，最后重新恢复数字的值返回即可。
// 代码如下：
int monotoneIncreasing(int num)
{
		vector<int>value;
		while (num>0)
		{
			value.push_back(num % 10);
			num = num / 10;
		}
		int i = 0;
		int j = value.size() - 1;
		while (i<j)
		{
			int tmp = value[i];
			value[i] = value[j];
			value[j] = tmp;
			i++;
			j--;
		}
		bool flag = false;
		for (i=0;i<value.size()-1;i++)
		{
			if (value[i] > value[i + 1])
			{
				flag = true;
				break;
			}
		}
		if (flag)
		{
			value[i] -= 1;
			for (int k=i;k>0;k--)
			{
				if (value[k] < value[k - 1])
				{
					value[k] = 9;
					value[k - 1] -= 1;
				}
			}
			for (int k=i+1;k<value.size();k++)
			{
				value[k] = 9;
			}
		}
		i = 0;
		j = value.size() - 1;
		while (i < j)
		{
			int tmp = value[i];
			value[i] = value[j];
			value[j] = tmp;
			i++;
			j--;
		}
		int pow = 1;
		int sum = 0;
		for (int k=0;k<value.size();k++)
		{
			sum += value[k] * pow;
			pow *= 10;
		}
		return sum;
}
// 714. 买卖股票的最佳时机含手续费
// 给定一个整数数组 prices，其中第 i 个元素代表了第 i 天的股票价格 ；非负整数 fee 代表了交易股票的手续费用。

// 你可以无限次地完成交易，但是你每笔交易都需要付手续费。如果你已经购买了一个股票，在卖出它之前你就不能再继续购买股票了。

// 返回获得利润的最大值。

// 注意：这里的一笔交易指买入持有并卖出股票的整个过程，每笔交易你只需要为支付一次手续费。
// 这里当遇到比历史记录中的最小买入价格小时，进行买入，相当于开始了新的一轮交易，而过去的收益已经记录，当出现比minSell+fee大时我们记录对应的收益即可，但是需要将minSell改为prices[i]-fee,这里相当于时假买入，我们累加到达未来最大卖出价格前的差分收益，等到遇到最大的卖出时机时此时收益为最低买入与收益区间中最大卖出价格的差值。
// 代码如下：
int maxProfitFee(vector<int>&nums,int fee)
	{
		int minSell = nums[0];
		int result = 0;
		for (int i=1;i<nums.size();i++)
		{
			if (nums[i] < minSell)minSell = nums[i];
			if (nums[i] > minSell && nums[i] < minSell + fee)continue;
			if (nums[i] > minSell + fee)
			{
				result += nums[i] - minSell - fee;
				minSell = nums[i] - fee;
			}
		}
		return result;
}
