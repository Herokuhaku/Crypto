#include <iostream>
#include <vector>
#include <sstream>

// ハッシュ生成: データから固有のハッシュを生成
std::string hashFunction(const std::string& input) {
    unsigned long hashValue = 7731;
    for (char character : input) {
        // DJB2ハッシュアルゴリズム
        // 32倍+ハッシュだから33倍でもいいけどシフトの方が早いっぽい
        hashValue = ((hashValue << 5) + hashValue) + character;
    }
    std::stringstream output;
    output << hashValue;
    return output.str();
}

// ブロック定義
class Block {
public:
    int position;
    std::string lastHash;
    std::string time;
    std::string content;
    std::string currentHash;
    // Posとかで作ればバリデータとかも必要ぽい

    // ブロック初期化
    Block(int pos, const std::string& prevHash, const std::string& timeStamp, const std::string& blockContent)
        : position(pos), lastHash(prevHash), time(timeStamp), content(blockContent) {
        currentHash = computeHash();
    }

    // ハッシュ計算: ブロックの内容からハッシュを計算してリターンする
    std::string computeHash() const {
        std::stringstream combinedData;
        combinedData << position << lastHash << time << content;
        return hashFunction(combinedData.str());
    }

    // ブロック表示するだけのやつ
    void show() const {
        std::cout << "Position: " << position << "\n"
            << "Time: " << time << "\n"
            << "Content: " << content << "\n"
            << "Hash: " << currentHash << "\n"
            << "Last Hash: " << lastHash << "\n"
            << "---------------------------" << "\n";
    }
};

// ブロックチェーン定義
class Blockchain {
private:
    std::vector<Block> blocks;

public:
    // 初期ブロック追加
    Blockchain() {
        blocks.push_back(startingBlock());
    }

    // ジェネシスブロック生成
    Block startingBlock() {
        return Block(0, "0", "2023-08-10", "Starting Block");
    }

    // 最新ブロック取得
    Block lastBlock() const {
        return blocks.back();
    }

    // ブロック追加
    void insert(const Block& block) {
        blocks.push_back(block);
    }

    // 有効性チェック
    bool isChainValid() const {
        for (int i = 1; i < blocks.size(); i++) {
            if (blocks[i].currentHash != blocks[i].computeHash() ||
                blocks[i].lastHash != blocks[i - 1].currentHash) {
                return false;
            }
        }
        return true;
    }

    // 全ブロック表示
    void showAll() const {
        for (const Block& block : blocks) {
            block.show();
        }
    }
};

// メイン実行部
int main() {
    Blockchain myBlockchain;

    myBlockchain.insert(Block(1, myBlockchain.lastBlock().currentHash, "2023-08-11", "Block 1"));
    myBlockchain.insert(Block(2, myBlockchain.lastBlock().currentHash, "2023-08-12", "Block 2"));

    myBlockchain.showAll();

    std::cout << "ブロックチェーンがある : true, ない : false →「" << (myBlockchain.isChainValid() ? "true" : "false") << "」" << std::endl;

    return 0;
}
