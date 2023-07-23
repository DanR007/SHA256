#include <iostream>
#include <vector>
#include <string>


std::string ToBinaryFromASCII(const std::string& ascii);
std::string ToBinaryFromDec(int num);
std::string ToBinaryFromDecH(uint64_t num);
uint64_t ToDecFromBinary(const std::string& bin);
std::string ToHexFromDec(uint64_t num);

void FillTo512Bit(std::string& bin);
std::vector <uint64_t> SplitBy32Bit(const std::string& bin);
void ChangeMessages(std::vector<std::string>& w);
void ChangeMessages(std::vector<uint64_t>& w);
void CombiningHash(std::string& hash);

uint64_t RightShift(const uint64_t& num, int shift);
uint64_t RightRotate(const uint64_t& num, int rotate);

void Compression(std::vector<uint64_t>& w);

void Print(const std::string& s)
{
    std::cout << s + "\n";
}

std::vector<uint64_t> h_arr =
{
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19
};

std::vector<uint64_t> k =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

int main(int argc, char** argv)
{
    std::string hash;
    if(argc > 1)
        hash = ToBinaryFromASCII(argv[1]);
    else
        hash = ToBinaryFromASCII("");

    FillTo512Bit(hash);
    std::vector<uint64_t> mes_num = SplitBy32Bit(hash);
    ChangeMessages(mes_num);
    Compression(mes_num);
    hash = "";
    CombiningHash(hash);

    Print("Hash is: ");
    Print(hash);

    system("pause");
}

void FillTo512Bit(std::string& bin)
{
    int bin_size = bin.size();
    bin += '1';

    while (bin.size() < 448)
    {
        bin += '0';
    }

    std::string start_size_bin = ToBinaryFromDec(bin_size);

    while (start_size_bin.size() + bin.size() < 512)
    {
        bin += '0';
    }

    bin += start_size_bin;

}
std::vector <uint64_t> SplitBy32Bit(const std::string& bin)
{
    std::vector <uint64_t> splited_bin;

    for (uint64_t i = 0; i < bin.size(); i += 32)
    {
        splited_bin.push_back(ToDecFromBinary(bin.substr(i, 32)));
    }

    for (uint64_t i = 0; i < 48; i++)
    {
        splited_bin.push_back(uint64_t(0));
    }

    return splited_bin;
}
void ChangeMessages(std::vector<uint64_t>& w)
{
    for (int i = 16; i < w.size(); i++)
    {
        uint64_t w_15 = w[i - 15];
        uint64_t w_16 = w[i - 16];
        uint64_t w_2 = w[i - 2];
        uint64_t w_7 = w[i - 7];

        uint64_t s0, s1;

        //s0 = (w[i-15] rightrotate 7) xor (w[i-15] rightrotate 18) xor (w[i-15] rightshift 3)
        s0 = RightRotate(w_15, 7) ^ RightRotate(w_15, 18) ^ RightShift(w_15, 3);

        //s1 = (w[i - 2] rightrotate 17) xor (w[i - 2] rightrotate 19) xor (w[i - 2] rightshift 10)
        s1 = RightRotate(w_2, 17) ^ RightRotate(w_2, 19) ^ RightShift(w_2, 10);

        //w[i] = w[i-16] + s0 + w[i-7] + s1 by 2^32 mod
        w[i] = (w_16 + s0 + w_7 + s1) & uint64_t(std::pow(2, 32) - 1);
    }
}
void Compression(std::vector<uint64_t>& w)
{
    uint64_t a, b, c, d, e, f, g, h;
    a = h_arr[0];
    b = h_arr[1];
    c = h_arr[2];
    d = h_arr[3];
    e = h_arr[4];
    f = h_arr[5];
    g = h_arr[6];
    h = h_arr[7];

    /*
    S1 = (e rightrotate 6) xor (e rightrotate 11) xor (e rightrotate 25)
    ch = (e and f) xor ((not e) and g)
    temp1 = h + S1 + ch + k[i] + w[i]
    S0 = (a rightrotate 2) xor (a rightrotate 13) xor (a rightrotate 22)
    maj = (a and b) xor (a and c) xor (b and c)
    temp2 := S0 + maj
    h = g
    g = f
    e = d + temp1
    d = c
    c = b
    b = a
    a = temp1 + temp2
    */
    for (int i = 0; i < w.size(); i++)
    {
        uint64_t s0, s1;
        uint64_t ch, temp1, temp2, maj;

        s1 = RightRotate(e, 6) ^ RightRotate(e, 11) ^ RightRotate(e, 25);

        ch = (e & f) ^ ((~e) & g);

        temp1 = (h + s1 + ch + k[i] + w[i]) & uint64_t(std::pow(2, 32) - 1);

        s0 = RightRotate(a, 2) ^ RightRotate(a, 13) ^ RightRotate(a, 22);

        maj = (a & b) ^ (a & c) ^ (b & c);

        temp2 = (s0 + maj) & uint64_t(std::pow(2, 32) - 1);

        h = g;
        g = f;
        f = e;
        e = (d + temp1) & uint64_t(std::pow(2, 32) - 1);
        d = c;
        c = b;
        b = a;
        a = (temp1 + temp2) & uint64_t(std::pow(2, 32) - 1);
    }

    h_arr[0] = (a + h_arr[0]) & uint64_t(std::pow(2, 32) - 1);
    h_arr[1] = (b + h_arr[1]) & uint64_t(std::pow(2, 32) - 1);
    h_arr[2] = (c + h_arr[2]) & uint64_t(std::pow(2, 32) - 1);
    h_arr[3] = (d + h_arr[3]) & uint64_t(std::pow(2, 32) - 1);
    h_arr[4] = (e + h_arr[4]) & uint64_t(std::pow(2, 32) - 1);
    h_arr[5] = (f + h_arr[5]) & uint64_t(std::pow(2, 32) - 1);
    h_arr[6] = (g + h_arr[6]) & uint64_t(std::pow(2, 32) - 1);
    h_arr[7] = (h + h_arr[7]) & uint64_t(std::pow(2, 32) - 1);
}

void CombiningHash(std::string& hash)
{
    for (uint64_t num : h_arr)
    {
        hash += ToHexFromDec(num);
    }
}

uint64_t RightShift(const uint64_t& num, int shift)
{
    return num >> shift;
}
uint64_t RightRotate(const uint64_t& num, int rotate)
{
    return (num >> rotate) | (num << (32 - rotate));
}


std::string ToHexFromDec(uint64_t num)
{
    const std::string hex_sys = "0123456789abcdef";
    std::string hex_rev = "", hex = "";
    while (num > 0)
    {
        hex_rev += hex_sys[num % 16];
        num /= 16;
    }
    for (int i = 0; i < 8 - hex_rev.size(); i++)
        hex += '0';

    for (int i = hex_rev.size() - 1; i >= 0; i--)
        hex += hex_rev[i];

    return hex;
}
std::string ToBinaryFromASCII(const std::string& ascii)
{
    std::string binary_from_ascii = "";
    for (char c : ascii)
    {
        std::string bin = ToBinaryFromDec(int(c));
        binary_from_ascii += bin;
    }
    return binary_from_ascii;
}
std::string ToBinaryFromDec(int num)
{
    std::string str = "";
    std::string binary = "";
    while (num > 0)
    {
        str += ('0' + num % 2);
        num /= 2;
    }
    for (int i = str.size() - 1; i >= 0; i--)
    {
        binary += str[i];
    }
    if (str.size() < 8)
    {
        str = "";
        for (int i = binary.size(); i < 8; i++)
        {
            str += '0';
        }
        binary = str + binary;
    }
    return binary;
}
std::string ToBinaryFromDecH(uint64_t num)
{
    std::string str = "";
    std::string binary = "";
    while (num > 0)
    {
        str += ('0' + num % 2);
        num /= 2;
    }
    for (int i = str.size(); i < 32; i++)
    {
        binary += '0';
    }
    for (int i = str.size() - 1; i >= 0; i--)
    {
        binary += str[i];
    }
    return binary;
}
uint64_t ToDecFromBinary(const std::string& bin)
{
    uint64_t dec = 0;
    for (int i = 0; i < bin.size(); i++)
    {
        dec += uint64_t((bin[i] - '0') * uint64_t(std::powl(2, bin.size() - 1 - i)));
    }

    return dec;
}