// 1049. 最后一块石头的重量 II
// 有一堆石头，每块石头的重量都是正整数。

// 每一回合，从中选出任意两块石头，然后将它们一起粉碎。假设石头的重量分别为 x 和 y，且 x <= y。那么粉碎的可能结果如下：

// 如果 x == y，那么两块石头都会被完全粉碎； 如果 x != y，那么重量为 x 的石头将会完全粉碎，而重量为 y 的石头新重量为 y-x。 最后，最多只会剩下一块石头。返回此石头最小的可能重量。如果没有石头剩下，就返回 0。
// 这里将石头分为两堆，如果两堆的重量组合相近，则剩下的差值就是最小的，这里使用石头总和的一半(向下取整)作为背包的容量，然后需要求解的问题是在这个背包中最多可以放入多重的石头，转化为了01背包问题
// 代码如下：
#include<vector>
#include<iostream>
#include<string>
#include<stack>
using namespace std;
int lastStone(vector<int>& stone)
{
		int sum = 0;
		for (int i=0;i<stone.size();i++)
		{
			sum += stone[i];
		}
		int n = sum / 2;
		vector<int>dp(n + 1);
		for (int i=0;i<=n;i++)
		{
			dp[i] = 0;
		}
		for (int i=0;i<stone.size();i++)
		{
			for (int j=n;j>0;j--)
			{
				if (stone[i] <= j)
				{
					dp[j] = max(dp[j], dp[j - stone[i]] + stone[i]);
				}
			}
		}
		return abs(dp[n] - (sum - dp[n]));
}
// 494. 目标和
// 给定一个非负整数数组，a1, a2, …, an, 和一个目标数，S。现在你有两个符号 + 和 -。对于数组中的任意一个整数，你都可以从 + 或 -中选择一个符号添加在前面。

// 返回可以使最终数组和为目标数 S 的所有添加符号的方法数。

// 将该数组分为左右两堆left和right,有
// left-right=target
// left+right=sum
// =>left=(target+sum)/2;
// 所以这里就是求在背包重量为**(target+sum)/2时，可以放入的石头方法的总数，转化为了01**背包问题
// 代码如下：
int TargetOfSum(vector<int>& nums,int S)
{
		int sum = 0;
		for (int i=0;i<nums.size();i++)
		{
			sum += nums[i];
		}
		if (abs(S) > sum)return 0;
		if ((S + sum) % 2 == 1)return 0;
		int left = (S + sum) / 2;
		vector<int>dp(left + 1);
		dp[0] = 1;
		for (int i=1;i<=left;i++)
		{
			dp[i] = 0;
		}
		for (int i=0;i<nums.size();i++)
		{
			for (int j=left;j>0;j--)
			{
				if (nums[i] <= j)
				{
					dp[j] += dp[j - nums[i]];
				}
			}
		}
		return dp[left];
}
// 474.一和零
// 给你一个二进制字符串数组 strs 和两个整数 m 和 n 。

// 请你找出并返回 strs 的最大子集的大小，该子集中 最多 有 m 个 0 和 n 个 1 。

// 如果 x 的所有元素也是 y 的元素，集合 x 是集合 y 的 子集 。

// 这道题属于二维上的01背包问题，m和n是两个方向上的背包，每个字符串就是一个物品，dp[i][j]表示在按顺序取得当前字符串时有i个0; j 个1时最多的子集个数
// 递推可以是
// dp[i][j]=max(dp[i][j],dp[i-zeroNum][j-oneNum]+1)
// 遍历和初始化类比一维数组实现的01背包问题
// 代码如下：
int oneAndZero(vector<string>& strs,int m,int n)
{
		vector<vector<int>>dp(m + 1, vector<int>(n+1));
		for (int i=0;i<=m;i++)
		{
			for (int j=0;j<=n;j++)
			{
				dp[i][j] = 0;
			}
		}
		for (string str:strs)
		{
			int zeroNum = 0;
			int oneNum = 0;
			for (int i=0;i<str.size();i++)
			{
				if (str[i] == '1')
					oneNum++;
				else
				{
					zeroNum++;
				}
			}
			for (int i=m;m>=zeroNum;i--)
			{
				for (int j=n;n>=oneNum;j--)
				{
					dp[i][j] = max(dp[i][j], dp[i - zeroNum][j - oneNum] + 1);
				}
			}
		}
		return dp[m][n];
}
