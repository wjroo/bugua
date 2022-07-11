/*
 * 函数命名：动词+操作的对象名称
 * 编写函数原则：尽量少做一点，简单一点，明确一点
 * 事实上一开始，从下往上搭建更自然一点
 * */

#include<ctime>
#include <algorithm>

using std::find;

#include <numeric>

using std::accumulate;
using std::transform;

#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::ostream;

#include <iomanip>

using std::setw;

#include <string>

using std::string;

#include <array>

using std::array;

#include <random>


class Divination {
public:

    void catalog_help() {
        int choose;
        while (true) {
            cout << "\n【选项目录】\n"
                 << "\t0：退出。\n"
                 << "\t1：蓍草法卜卦。\n"
                 << "\t2：硬币法卜卦。\n"
                 << "\t3：已知操作结果，从下往上输入6789（举例：778898），进行解卦。\n"
                 << "\t4：查询卦象，从下往上输入01（举例：100001）。\n"
                 << "\t5：切换是否暂停。\n";

            do {
                cout << "输入你的选择：" << endl;
                cin >> choose;
            } while (choose < 0 || choose > 5);

            int input_num = 0, num_dec, num_order;
            switch (choose) {
                case 0:
                    return;
                case 1:
                    generate_by_yarrow();
                    break;
                case 2:
                    generate_by_coin();
                    break;
                case 3:
                    do {
                        cout << "请输入：" << endl;
                        cin >> input_num;
                    } while (input_num < 666666 || input_num > 999999);
                    convert_strdec_array(input_num, bin6789);
                    break;
                case 4:
                    do {
                        cout << "请输入：" << endl;
                        cin >> input_num;
                    } while (input_num < 0 || input_num > 111111);
                    convert_strdec_array(input_num, bengua);
                    num_dec = convert_bin01_dec(bengua);
                    num_order = find_hexagrams_order(num_dec);

                    print_bin01(bengua);
                    print_hexagrams_name(num_order);
                    cout << "\t二进制卦象转十进制为：" << num_dec << "        卦序为：" << num_order << "\n" << endl;
                    break;
                case 5:
                    pause = !pause;
                    cout << "pause=" << std::boolalpha << pause << endl;
                    break;
                default:
                    break;
            }
            switch (choose) {
                case 1:
                case 2:
                case 3:
                    solve();
                    break;

                default:
                    break;
            }
        }
    }

private:

    array<int, 6> bin6789{};
    array<int, 6> bengua{};
    array<int, 6> biangua{};
    array<int, 6> cuogua{};
    array<int, 6> zonggua{};
    array<int, 6> jiaogua{};
    array<int, 6> hugua{};

    static const array<int, 64> hexagrams_order;

    static array<string, 64> hexagrams_name;

    bool pause = true;

    void generate_by_yarrow() {//蓍草法
        /*
         * 【对蓍草法的概率研究】
         * 已知有：
         * 第一次剩余49，如果随机数的连续跨度是4的倍数的话，减5和减9的概率比为3比1。
         * 第二次和第三次减4和减8的概率比为1比1。
         *
         * 则一次减去之和有2种情况：
         * 1、-5，概率3/4，49-5=44
         * 2、-9，概率1/4，49-9=40
         *
         * 则二次减去之和有3种情况：
         * 1、-9=-54，概率3/8，49-9=40
         * 2、-13=-58=-94，概率4/8，49-13=36
         * 3、-17=-98，概率1/8，49-17=32
         *
         * 则三次减去之和有4种情况：
         * 1、-13=-544，概率3/16，49-13=36
         * 2、-17=-944=-584=-548，概率7/16，49-17=32
         * 3、-21=-588=-948=-984，概率5/16，49-21=28
         * 4、-25=-988，概率1/16，49-25=24
         *
         *       1
         *      / \
         *     3   1
         *    / \ / \
         *   3   4   1
         *  / \ / \ / \
         * 3   7   5   1
         *
         */
        for (int i = 0; i < 6; ++i) {
            if (pause) {
                cout << "按回车键，卜爻 " << i + 1 << "：" << endl;
                cin.get();
            } else {
                cout << "卜爻 " << i + 1 << "：" << endl;
            }

            int remain = 49;
            for (int j = 1; j <= 3; ++j) {
                if (pause) {
                    cout << "\t按回车键，抽第 " << j << " 次：" << endl;
                    cin.get();
                } else {
                    cout << "\t抽第 " << j << " 次：" << endl;
                }

                int left_sky = random_gen(time(nullptr), remain / 3, remain / 3 + remain / 12 * 4 - 1);
                int right_earth = remain - left_sky - 1;
                int l_left = left_sky % 4;
                if (l_left == 0) {
                    l_left = 4;
                }
                int r_left = right_earth % 4;
                if (r_left == 0) {
                    r_left = 4;
                }
                int will_sub = l_left + r_left + 1;

                cout << "\t\t\t左为天 " << left_sky << "=4*" << (left_sky - l_left) / 4 << "+" << l_left
                     << "\t右为地 " << right_earth << "=4*" << (right_earth - r_left) / 4 << "+" << r_left << endl;
                cout << "\t\t" << remain << "-" << will_sub << "=" << remain - will_sub << endl;
                remain -= will_sub;
            }
            bin6789[i] = remain / 4;
            switch (i) {
                case 0:
                    cout << "初爻：" << bin6789[i] << endl;
                    break;
                case 5:
                    cout << "上爻：" << bin6789[i] << endl;
                    break;
                default:
                    cout << "爻 " << i + 1 << "：" << bin6789[i] << endl;
            }
        }
    }

    void generate_by_coin() {//硬币法
        /*
         * 【对硬币法的概率研究】
         * 易知3次掷硬币后求和为6、7、8、9的概率比为1:3:3:1。
         *
         *       1
         *      / \
         *     1   1
         *    / \ / \
         *   1   2   1
         *  / \ / \ / \
         * 1   3   3   1
         *
         */
        for (int i = 0; i < 6; ++i) {
            if (pause) {
                cout << "按回车键，卜爻 " << i + 1 << "：" << endl;
                cin.get();
            } else {
                cout << "卜爻 " << i + 1 << "：" << endl;
            }

            int score_sum = 0;
            for (int j = 1; j <= 3; ++j) {
                if (pause) {
                    cout << "\t按回车键，掷第 " << j << " 个硬币：" << endl;
                    cin.get();
                } else {
                    cout << "\t掷第 " << j << " 个硬币：" << endl;
                }

                int score_num = random_gen(time(nullptr), 2, 3);
                score_sum += score_num;

                if (score_num == 3) {
                    cout << "\t\t\t正面。" << endl;
                } else {
                    cout << "\t\t\t反面。" << endl;
                }
            }
            bin6789[i] = score_sum;
            switch (i) {
                case 0:
                    cout << "初爻：" << bin6789[i] << endl;
                    break;
                case 5:
                    cout << "上爻：" << bin6789[i] << endl;
                    break;
                default:
                    cout << "爻 " << i + 1 << "：" << bin6789[i] << endl;
            }
        }
    }

    void solve() {
        //本卦
        {
            convert_bin6789_bengua(bin6789, bengua);
            int num_dec = convert_bin6789_dec(bin6789);
            int num_order = find_hexagrams_order(num_dec);

            cout << "\t【本卦】" << endl;
            print_bin6789(bin6789);
            print_hexagrams_name(num_order);
            cout << "\t二进制卦象转十进制为：" << num_dec << "        卦序为：" << num_order << "\n" << endl;
        }

        {
            int sum_bin6789 = sum_array(bin6789);
            int diff = 55 - sum_bin6789;
            int quotient = diff / 6;
            int remainder = diff % 6;

            cout << "\t\t55-" << sum_bin6789 << "=" << diff << "=6*" << quotient << "+" << remainder << "    ";
            if (quotient % 2) {
                if (remainder) {
                    cout << "爻 " << 7 - remainder << "! * * *\n" << endl;
                } else {
                    cout << "爻 6! * * *\n" << endl;
                }
            } else {
                if (remainder) {
                    cout << "爻 " << remainder << "! * * *\n" << endl;
                } else {
                    cout << "爻 1! * * *\n" << endl;
                }
            }
        }

        // 变卦
        {
            if (has_cuogua_bin6789(bin6789)) {
                convert_bin6789_biangua(bin6789, biangua);
                int num_dec = convert_bin01_dec(biangua);
                int num_order = find_hexagrams_order(num_dec);

                cout << "\t【变卦】" << endl;
                print_bin01(biangua);
                print_hexagrams_name(num_order);
                cout << "\t二进制卦象转十进制为：" << num_dec << "        卦序为：" << num_order << "\n" << endl;
            } else {
                cout << "\t【变卦】=【本卦】\n" << endl;
            }
        }

        //错挂
        {
            convert_bin01_cuogua(bengua, cuogua);
            int num_dec = convert_bin01_dec(cuogua);
            int num_order = find_hexagrams_order(num_dec);

            cout << "\t【错卦】" << endl;
            print_bin01(cuogua);
            print_hexagrams_name(num_order);
            cout << "\t二进制卦象转十进制为：" << num_dec << "        卦序为：" << num_order << "\n" << endl;
        }

        //综卦
        {
            if (has_zonggua_bin01(bengua)) {
                convert_bin01_zonggua(bengua, zonggua);
                int num_dec = convert_bin01_dec(zonggua);
                int num_order = find_hexagrams_order(num_dec);

                cout << "\t【综卦】" << endl;
                print_bin01(zonggua);
                print_hexagrams_name(num_order);
                cout << "\t二进制卦象转十进制为：" << num_dec << "        卦序为：" << num_order << "\n" << endl;
            } else {
                cout << "\t【综卦】=【本卦】\n" << endl;
            }
        }

        //交卦
        {
            if (has_jiaogua_bin01(bengua)) {
                convert_bin01_jiaogua(bengua, jiaogua);
                int num_dec = convert_bin01_dec(jiaogua);
                int num_order = find_hexagrams_order(num_dec);

                cout << "\t【交卦】" << endl;
                print_bin01(jiaogua);
                print_hexagrams_name(num_order);
                cout << "\t二进制卦象转十进制为：" << num_dec << "        卦序为：" << num_order << "\n" << endl;
            } else {
                cout << "\t【交卦】=【本卦】\n" << endl;
            }
        }

        //互卦
        {
            if (has_hugua_bin01(bengua)) {
                convert_bin01_hugua(bengua, hugua);
                int num_dec = convert_bin01_dec(hugua);
                int num_order = find_hexagrams_order(num_dec);

                cout << "\t【互卦】" << endl;
                print_bin01(hugua);
                print_hexagrams_name(num_order);
                cout << "\t二进制卦象转十进制为：" << num_dec << "        卦序为：" << num_order << "\n" << endl;
            } else {
                cout << "\t【互卦】=【本卦】\n" << endl;
            }
        }
    }

    static int convert_bin01_dec(array<int, 6> &a) {
        int sum_ = 0;
        for (int iter: a) {
            sum_ = (sum_ << 1) + iter;
        }
        return sum_;
    }

    static void convert_bin01_cuogua(array<int, 6> &a, array<int, 6> &b) {
        bool error = false;
        transform(a.cbegin(), a.cend(), b.begin(), [&error](int dec_num) {
            switch (dec_num) {
                case 1:
                    return 0;
                case 0:
                    return 1;
                default:
                    error = true;
                    return dec_num;
            }
        });
        if (error) {
            std::for_each(a.cbegin(), a.cend(), [](int num_) { std::cerr << num_ << " "; });
            std::cerr << "out_of_range" << endl;
            cin.get();
        }
    }

    static void convert_bin01_zonggua(array<int, 6> &a, array<int, 6> &b) {
        std::copy(a.crbegin(), a.crend(), b.begin());
    }

    static void convert_bin01_jiaogua(array<int, 6> &a, array<int, 6> &b) {
        std::copy(a.cbegin(), a.cbegin() + a.size() / 2, b.begin() + a.size() / 2);
        std::copy(a.cbegin() + a.size() / 2, a.cend(), b.begin());
    }

    static void convert_bin01_hugua(array<int, 6> &a, array<int, 6> &b) {
        std::copy(a.cbegin() + 1, a.cbegin() + a.size() / 2 + 1, b.begin());
        std::copy(a.crbegin() + 1, a.crbegin() + (int) a.size() / 2 + 1, b.rbegin());
    }

    static int convert_bin6789_dec(array<int, 6> &a) {
        int sum_ = 0;
        for (auto riter = a.crbegin(); riter != a.crend(); ++riter) {
            switch (*riter) {
                case 6:
                case 8:
                    break;
                case 7:
                case 9:
                    sum_ += 1 << (riter - a.crbegin());
                    break;
                default:
                    cout << "Error!" << endl;
            }
        }
        return sum_;
    }

    static void convert_bin6789_bengua(array<int, 6> &a, array<int, 6> &b) {
        bool error = false;
        transform(a.cbegin(), a.cend(), b.begin(), [&error](int dec_num) {
            switch (dec_num) {
                case 6:
                case 8:
                    return 0;
                case 7:
                case 9:
                    return 1;
                default:
                    error = true;
                    return dec_num;
            }
        });
        if (error) {
            std::for_each(a.cbegin(), a.cend(), [](int num_) { std::cerr << num_ << " "; });
            std::cerr << "out_of_range" << endl;
            cin.get();
        }
    }

    static void convert_bin6789_biangua(array<int, 6> &a, array<int, 6> &b) {
        bool error = false;
        transform(a.cbegin(), a.cend(), b.begin(), [&error](int dec_num) {
            switch (dec_num) {
                case 6:
                case 7:
                    return 1;
                case 8:
                case 9:
                    return 0;
                default:
                    error = true;
                    return dec_num;
            }
        });
        if (error) {
            std::for_each(a.cbegin(), a.cend(), [](int num_) { std::cerr << num_ << " "; });
            std::cerr << "out_of_range" << endl;
            cin.get();
        }
    }

    static void convert_strdec_array(int strdec, array<int, 6> &a) {
        for (auto riter = a.rbegin(); riter != a.rend(); ++riter) {
            *riter = strdec % 10;
            strdec /= 10;
        }
    }

    static int find_hexagrams_order(int dec_num) {
        auto found = find(hexagrams_order.cbegin(), hexagrams_order.cend(), dec_num);
        if (found == hexagrams_order.cend()) {
            cout << "未找到元素。" << endl;
        }
        return int(found - hexagrams_order.cbegin() + 1);
    }

    static bool has_cuogua_bin6789(array<int, 6> &a) {
        return std::any_of(a.cbegin(), a.cend(), [](int x) { return x == 6 || x == 9; });
    }

    static bool has_zonggua_bin01(array<int, 6> &a) {
        auto iter = a.cbegin();
        auto riter = a.crbegin();
        for (; iter != riter.base(); ++iter, ++riter) {
            if (*iter != *riter) {
                return true;
            }
        }
        return false;
    }

    static bool has_jiaogua_bin01(array<int, 6> &a) {
        for (auto iter = a.cbegin(), miditer = iter + a.size() / 2; miditer != a.cend(); ++iter, ++miditer) {
            if (*iter != *miditer) {
                return true;
            }
        }
        return false;
    }

    static bool has_hugua_bin01(array<int, 6> &a) {
        for (auto iter = a.cbegin(), stop = iter + a.size() / 2; iter != stop; ++iter) {
            if (*iter != *(iter + 1)) {
                return true;
            }
        }
        for (auto riter = a.crbegin(), stop = riter + (int) a.size() / 2; riter != stop; ++riter) {
            if (*riter != *(riter + 1)) {
                return true;
            }
        }
        return false;
    }

    static void print_bin01(array<int, 6> &a) {
        for (auto riter = a.crbegin(); riter != a.crend(); ++riter) {
            switch (*riter) {
                case 0:
                    cout << "\t\t___   ___" << endl;
                    break;
                case 1:
                    cout << "\t\t_________" << endl;
                    break;
                default:
                    cout << "Error!" << endl;
            }
        }
    }

    static void print_bin6789(array<int, 6> &a) {
        for (auto riter = a.crbegin(); riter != a.crend(); ++riter) {
            switch (*riter) {
                case 6:
                    cout << "\t\t___   ___\t老 阴 " << *riter << endl;
                    break;
                case 7:
                    cout << "\t\t_________\t少 阳 " << *riter << endl;
                    break;
                case 8:
                    cout << "\t\t___   ___\t少 阴 " << *riter << endl;
                    break;
                case 9:
                    cout << "\t\t_________\t老 阳 " << *riter << endl;
                    break;
                default:
                    cout << "Error!" << endl;
            }
        }
    }

    static void print_hexagrams_name(int order) {
        cout << "\n\t\t" << hexagrams_name[order - 1] << endl;
    }

    static int sum_array(array<int, 6> &a) {
        return accumulate(a.cbegin(), a.cend(), 0);
    }

    static int random_gen(unsigned seed = time(nullptr), int min = 0, int max = std::numeric_limits<int>::max()) {
        static std::random_device seed_hardware;
        static std::default_random_engine e(seed ^ seed_hardware());
        static std::uniform_int_distribution<int> ud(min, max);
        ud.param(std::uniform_int_distribution<int>::param_type{min, max});
        return ud(e);
    }

};

const array<int, 64> Divination::hexagrams_order = {
        63, 0, 34, 17, 58,
        23, 16, 2, 59, 55,
        56, 7, 47, 61, 8,
        4, 38, 25, 48, 3,
        37, 41, 1, 32, 39,
        57, 33, 30, 18, 45,
        14, 28, 15, 60, 5,
        40, 43, 53, 10, 20,
        49, 35, 62, 31, 6,
        24, 22, 26, 46, 29,
        36, 9, 11, 52, 44,
        13, 27, 54, 19, 50,
        51, 12, 42, 21
};

array<string, 64> Divination::hexagrams_name = {
        "Qian 乾 天", "Kun 坤 地", "Zhun 水雷 屯", "Meng 山水 蒙", "Xu 水天 需",
        "Song 天水 讼", "Shi 地水 师", "Bi 水地 比", "Xiao Xu 风天 小畜", "Lv 天泽 履",
        "Tai 地天 泰", "Pi 天地 否", "Tong Ren 天火 同人", "Da You 火天 大有", "Qian 地山 谦",
        "Yu 雷地 豫", "Sui 泽雷 随", "Gu 山风 蛊", "Lin 地泽 临", "Guan 风地 观",
        "Shi Ke 火地 噬嗑", "Bi 山火 贲", "Bo 山地 剥", "Fu 地雷 复", "Wu Wang 天雷 无妄",
        "Da Xu 山天 大畜", "Yi 山雷 颐", "Da Guo 泽风 大过", "Kan 坎 水", "Li 离 火",
        "Xian 泽山 咸", "Heng 雷风 恒", "Dun 天山 遁", "Da Zhuang 雷天 大壮", "Jin 火地 晋",
        "Ming Yi 地火 明夷", "Jia Ren 风火 家人", "Kui 火泽 睽", "Jian 水山 蹇", "Jie 雷水 解",
        "Sun 山泽 损", "Yi 风雷 益", "Guai 泽天 夬", "Gou 天风 姤", "Cui 泽地 萃",
        "Sheng 地风 升", "Kun 泽水 困", "Jing 水风 井", "Ge 泽火 革", "Ding 火风 鼎",
        "Zhen 震 雷", "Gen 艮 山", "Jian 风山 渐", "Gui Mei 雷泽 归妹", "Feng 雷火 丰",
        "Lv 火山 旅", "Xun 巽 风", "Dui 兑 泽", "Huan 风水 涣", "Jie 水泽 节",
        "Zhong Fu 风泽 中孚", "Xiao Guo 雷山 小过", "Ji Ji 水火 既济", "Wei Ji 火水 未济"
};


int main() {
    Divination bugua = Divination();

    bugua.catalog_help();
}

