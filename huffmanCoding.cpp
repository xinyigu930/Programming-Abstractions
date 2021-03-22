#include "Huffman.h"
#include "GUI/SimpleTest.h"
#include "hashmap.h"
#include "vector.h"
#include "priorityqueue.h"
#include <string>
#include "map.h"
using namespace std;
/* This program implements Huffman coding to compress and decompress files.*/

/**
 * Deallocates all nodes in a Huffman tree. We've provided this helper function
 * to you since we also use it in our test driver and figured you might want
 * to make use of it.
 */
void deleteTree(EncodingTreeNode* tree) {
    if (tree != nullptr) {
        deleteTree(tree->zero);
        deleteTree(tree->one);
        delete tree;
    }
}

/**
 * Given a string, builds a character frequency map that maps
 * each different character to the number of items the chacter
 * appears in the string.
 */
void BuildFrequencyMap(Map<char,int>& frequency, const string& str) {
    for (int i = 0; i < int(str.size()); i++ ) {

        /* If the map doesn't have this character yet, make a new entry.  */
        if (!frequency.containsKey(str[i])) {
            frequency[str[i]] = 1;

        }
        /* Otherwise, increases this character's frequency by 1.  */
        else {
            frequency[str[i]] ++;
        }
    }
}

/**
 * Given a priority queue of EncodingTreeNodes, recursively combines two nodes
 * until only a single node exists.
 */
void combineRec(PriorityQueue<EncodingTreeNode*>& pq) {
    /* Base case: we have only one super node left! */
    if (pq.size() == 1) {
        return;
    }
    /* Recursive case: two or more nodes are present in the priority queue. */
    else {
        int freqA = pq.peekPriority();
        EncodingTreeNode* zero = pq.dequeue();
        int freqB = pq.peekPriority();
        EncodingTreeNode* one = pq.dequeue();
        /* Create a new parent node. */
        EncodingTreeNode* parent = new EncodingTreeNode;
        parent->zero = zero;
        parent->one = one;
        /* Sets the parent's new priority as the sum of two children's priorities. */
        pq.enqueue(parent, freqA + freqB);
        combineRec(pq);
    }
}

/**
 * Constructs a Huffman coding tree for the given string, using the algorithm
 * described in lecture.
 *
 * If the input string does not contain at least two different characters,
 * this function should report an error.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 */
EncodingTreeNode* huffmanTreeFor(const string& str) {
    Map<char,int> frequency;
    BuildFrequencyMap(frequency, str);

    /* Prints an error message if the string has less than 2 different characters.  */
    if (frequency.size() <= 1) {
        error("The input string should have at least two different characters.");
    }
    PriorityQueue<EncodingTreeNode*> pq;

    /* Stores each character's value in a new EncodingTreeNode
     * and enqueue it to the priority queue.  */
    for (char key : frequency) {
        EncodingTreeNode* node = new EncodingTreeNode;
        node->ch = key;
        node->one = nullptr;
        node->zero = nullptr;
        pq.enqueue(node, frequency[key]);
    }

    /* Recursively builds a huffman tree. */
    combineRec(pq);
    return pq.dequeue();
}

/**
 * Given a Queue<Bit> containing a compressed message and a tree that was used
 * to encode those bits, decodes the bits back to the original message.
 *
 * You can assume the input tree is not null and is not a single character;
 * those are edge cases you don't need to handle.
 *
 * You can assume the input Queue<Bit> is well-formed in that every character
 * was encoded correctly, there are no stray bits in the Queue, etc.
 */
string decodeText(Queue<Bit>& bits, EncodingTreeNode* tree) {
    string result = "";

    /* Root points at the very top of the tree. */
    EncodingTreeNode* root = tree;
    while (bits.size()!= 0) {
        Bit b = bits.dequeue();

        /* The tree pointer moves following the direction given by the enqueued Bit. */
        b == 0 ? tree = tree->zero : tree = tree->one;

        /* We are at a leaf of the tree! */
        if ( tree->one == nullptr) {
            result += tree->ch;

            /* After we've found a character, reset the tre pointer to its root. */
            tree = root;
        }
    }
    return result;
}


/**
 * Given a Huffman encoding tree, creats a map that pairs each character
 * in a leaf to its bit sequence. Uses recursion to track the path from
 * the root to each leaf.
 */
void mapCharBits(EncodingTreeNode* tree, Map<char, Vector <Bit>>& map, Vector <Bit> path) {
    /* Base case: we've reached a leaf node! */
    if (tree->one == nullptr) {

        /* Add the pair to the map. */
        map[tree->ch] = path;
        return;
    }
    /* Recursive case 1: take one step to the left child node */
    mapCharBits(tree->zero, map, path + 0);

    /* Recursive case 2: take one step to the right child node */
    mapCharBits(tree->one, map, path + 1);
}

/**
 * Given a string and a Huffman encoding tree, encodes that text using the tree
 * and outputs a Queue<Bit> corresponding to the encoded representation.
 *
 * The input tree will not be null and will not consist of a single node; these
 * are edge cases you don't have to handle. The input tree will contain all
 * characters that make up the input string.
 */
Queue<Bit> encodeText(const string& str, EncodingTreeNode* tree) {
    Queue <Bit> encoded = {};
    Map<char, Vector <Bit>> map;
    Vector <Bit> path = {};

    /* Creats a map that pairs each character in a leaf to its bit sequence. */
    mapCharBits(tree, map, path);

    /* For each character in str, retrieve its bit sequence from the map above,
     *  and enqueue the bits into the queue, which the function will return. */
    for (int i = 0; i < (int)str.size(); i++) {
        for (Bit elem : map[str[i]]) {
            encoded.enqueue(elem);
        }
    }
    return encoded;
}


/**
 *Given a Queue<Bit> representing the shape of the tree and a Queue<char>
 *representing the characters stored in the tree leaves, recursively builds
 *a Huffman coding tree.
 */
void decodeTreeRec(Queue<Bit>& bits, Queue<char>& leaves, EncodingTreeNode* tree){
    Bit b = bits.dequeue();

    /* Base case: the current node is a leaf node!  */
    if (b == 0) {
        tree->one = nullptr;
        tree->zero = nullptr;
        tree->ch = leaves.dequeue();
        return;
    }

    /* Otherwise, if the current node is an internal node, create two child nodes. */
    EncodingTreeNode* left = new EncodingTreeNode;
    EncodingTreeNode* right = new EncodingTreeNode;
    tree->zero = left;
    tree->one = right;

    /* Recursive case 1: move the current pointer to its left child node. */
    decodeTreeRec(bits, leaves, tree->zero);

    /* Recursive case 2: move the current pointer to its right child node. */
    decodeTreeRec(bits, leaves, tree->one);
}


/**
 * Decodes the given Queue<Bit> and Queue<char> into a Huffman coding tree.
 *
 * You can assume that the input Queues are structured properly in that they
 * represent a legal encoding of a tree, that there aren't stray characters
 * or bits in them, etc.
 */
EncodingTreeNode* decodeTree(Queue<Bit>& bits, Queue<char>& leaves) {
    EncodingTreeNode* tree = new EncodingTreeNode;
    EncodingTreeNode* root = tree;

    /* Recursively builds a Huffman coding tree. */
    decodeTreeRec(bits, leaves, tree);
    return root;
}

/**
 * Encodes the given Huffman tree as a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment handout.
 *
 * You can assume the input Queues are empty on entry to this function.
 *
 * You can assume that the Huffman tree provided to you is properly structured,
 * in that each internal node will have two children and only the characters in
 * the leaves matter, etc.
 */
void encodeTree(EncodingTreeNode* tree, Queue<Bit>& bits, Queue<char>& leaves) {

    /* If the current node is a single leaf node, store its character to the
     * Queue leaves and 0 to the Queue bits. */
    if (tree->one == nullptr) {
        leaves.enqueue(tree->ch);
        bits.enqueue(0);
        return;
    }

    /*Otherwise, stores 1 to the Queue bits and recursively examines its two child nodes.  */
    bits.enqueue(1);
    encodeTree(tree->zero, bits, leaves);
    encodeTree(tree->one, bits, leaves);
}


/**
 * Compresses the given text string using Huffman coding, producing as output
 * a HuffmanResult containing the encoded tree and message.
 *
 * Your implementation of this function should report an error if there are
 * fewer than two distinct characters in the input string.
 */
HuffmanResult compress(const string& text) {
    HuffmanResult result;

    /* Given the text, build a huffman tree! */
    EncodingTreeNode* tree = huffmanTreeFor(text);

    /* Uses the huffman tree to encode the given text to an encoded messageBits. */
    result.messageBits = encodeText(text, tree);

    /* Encodes the given Huffman tree as a Queue<Bit> and Queue<char>. */
    Queue<Bit> bits;
    Queue<char> leaves;
    encodeTree(tree, bits, leaves);
    result.treeBits = bits;
    result.treeLeaves = leaves;

    /*Frees the memory allocated to the huffman tree! */
    deleteTree(tree);
    return result;
}

/**
 * Decompresses the given HuffmanResult and returns the string it represents.
 *
 * Your implementation may change the file parameter however it sees fit. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * You can assume the input file is well-formed and was created by a correct
 * implementation of compress.
 */
string decompress(HuffmanResult& file) {
    Queue<Bit> bits = file.treeBits;
    Queue<char> leaves = file.treeLeaves;

    /*Creates a huffman tree from the two queues above. */
    EncodingTreeNode* tree = decodeTree(bits, leaves);
    Queue<Bit> messageBits = file.messageBits;
    string result;

    /*Uses the tree to decode the encoded messageBits into a string. */
    result = decodeText(messageBits, tree);

    /*Frees the memory allocated to the huffman tree! */
    deleteTree(tree);
    return result;
}



/* * * * * * Test Cases Below This Point * * * * * */
bool isEncodingTree(EncodingTreeNode* tree);
bool areEqual(EncodingTreeNode* lhs, EncodingTreeNode* rhs);
STUDENT_TEST("huffmanTreeFor uses cumulative weights.") {
    /*
     *          *
     *       /     \
     *      *       *
     *     / \     / \
     *    p   o   *   m
     *           / \
     *          !   *
     *             / \
     *            e   q
     */

    EncodingTreeNode* reference =new EncodingTreeNode {
            ' ',
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'p', nullptr, nullptr },
            new EncodingTreeNode{ 'o', nullptr, nullptr }
},
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ '!', nullptr, nullptr },
            new EncodingTreeNode{
            ' ',
            new EncodingTreeNode{ 'e', nullptr, nullptr },
            new EncodingTreeNode{ 'q', nullptr, nullptr },
}
},
            new EncodingTreeNode{ 'm', nullptr, nullptr },
}
};
    /* mx5, ox4, px3, qx2, !x2, ex1 */
    EncodingTreeNode* tree = huffmanTreeFor("mmmmmoooopppqq!!e");
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

STUDENT_TEST("decodeText works on non-letter characters.") {
    /*
     *           *
     *         /   \
     *        o     *
     *             / \
     *            !   *
     *               / \
     *              *   d
     *             / \
     *            w   r
     */

    EncodingTreeNode* tree =new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'o', nullptr, nullptr },
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ '!', nullptr, nullptr },
            new EncodingTreeNode{
            ' ',
            new EncodingTreeNode{
            ' ',
            new EncodingTreeNode{ 'w', nullptr, nullptr },
            new EncodingTreeNode{ 'r', nullptr, nullptr },
},
            new EncodingTreeNode{ 'd', nullptr, nullptr },

}
}
};

    /* What you get if you encode "wooddoor!!" with this tree. */
    Queue<Bit> bits = { 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0};

    EXPECT_EQUAL(decodeText(bits, tree), "wooddoor!!");

    deleteTree(tree);
}

STUDENT_TEST("encodeText works on non-letter characters.") {
    /*
     *           *
     *         /   \
     *        o     *
     *             / \
     *            ?   *
     *               / \
     *              *   d
     *             / \
     *            w   r
     */

    EncodingTreeNode* tree =new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'o', nullptr, nullptr },
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ '?', nullptr, nullptr },
            new EncodingTreeNode{
            ' ',
            new EncodingTreeNode{
            ' ',
            new EncodingTreeNode{ 'w', nullptr, nullptr },
            new EncodingTreeNode{ 'r', nullptr, nullptr },
},
            new EncodingTreeNode{ 'd', nullptr, nullptr },

}
}
};

    /* What you get if you encode "wooddoor??" with this tree. */
    Queue<Bit> expected = { 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0};

    EXPECT_EQUAL(encodeText("wooddoor??", tree), expected);

    deleteTree(tree);
}

STUDENT_TEST("Can encode a tree with various characters.") {
    /* Build an encoding tree for "i[[}}}&&&~~~~55555^^^^^^^^^^^^^^^^^^" It should look like this:
     *          *
     *        /   \
     *       *     ^
     *     /   \
     *    *     *
     *   / \   / \
     *  &  ~  5   *
     *           / \
     *          *   }
     *         / \
     *        i   [
     *
     *
     * This will compress down to
     *
     *        1110010110000
     *        &~5i[}^
     */
    EncodingTreeNode* tree = huffmanTreeFor("i[[}}}&&&~~~~55555^^^^^^^^^^^^^^^^^^");

    Queue<Bit>  bits;
    Queue<char> leaves;

    encodeTree(tree, bits, leaves);

    Queue<Bit>  expectedBits   = { 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0 };
    Queue<char> expectedLeaves = { '&', '~', '5', 'i', '[', '}', '^' };

    EXPECT_EQUAL(bits,   expectedBits);
    EXPECT_EQUAL(leaves, expectedLeaves);

    deleteTree(tree);
}

STUDENT_TEST("decodeTree undoes encodeTree on sample strings.") {
    /* Make a Huffman tree for the string of all characters. */
    EncodingTreeNode* sourceTree = huffmanTreeFor("i[[}}}&&&~~~~55555^^^^^^^^^^^^^^^^^^");
    EXPECT(isEncodingTree(sourceTree));

    /* Encode, then decode it. */
    Queue<Bit>  bits;
    Queue<char> leaves;
    encodeTree(sourceTree, bits, leaves);

    EncodingTreeNode* resultTree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(resultTree));
    EXPECT(areEqual(sourceTree, resultTree));

    deleteTree(sourceTree);
    deleteTree(resultTree);
}

STUDENT_TEST("Can decode an tree with various characters.") {
    /* This encodes this tree:
     *
     *           *
     *         /   \
     *        o     *
     *             / \
     *            ?   *
     *               / \
     *              *   d
     *             / \
     *            w   r
     */

    Queue<Bit>  bits   = { 1, 0, 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> leaves = { 'o', '?', 'w', 'r', 'd' };

    EncodingTreeNode* tree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(tree));

    /* Confirm this is the right tree. */
    EncodingTreeNode* expected =new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'o', nullptr, nullptr },
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ '?', nullptr, nullptr },
            new EncodingTreeNode{
            ' ',
            new EncodingTreeNode{
            ' ',
            new EncodingTreeNode{ 'w', nullptr, nullptr },
            new EncodingTreeNode{ 'r', nullptr, nullptr },
},
            new EncodingTreeNode{ 'd', nullptr, nullptr },

}
}
};

    EXPECT(areEqual(tree, expected));

    deleteTree(tree);
    deleteTree(expected);
}

STUDENT_TEST("Can decompress a sample file with various chacters.") {
    HuffmanResult file = {
        {1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0},
        { '+', '$', '@', '<', '6', '>', 'B', 'a' },
        { 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1,
          0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1,
          1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1}
    };
    EXPECT_EQUAL(decompress(file), "a6B><$@a+6>BaaB66a");
}

STUDENT_TEST("Can compress a file with two repetitive characters.") {
    HuffmanResult file = compress("HiiiiiiiiiiiiiiiiiiiiHHHiHiHHHHHHHHHHHH");
    Queue<Bit>  treeBits    = { 1, 0, 0 };
    Queue<char> treeChars   = { 'H', 'i'};
    Queue<Bit>  messageBits = { 0, 1, 1, 1, 1, 1, 1, 1, 1,
                                1, 1, 1, 1, 1, 1, 1, 1, 1,
                                1, 1, 1, 0, 0, 0, 1, 0, 1,
                                0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0};

    EXPECT_EQUAL(file.treeBits, treeBits);
    EXPECT_EQUAL(file.treeLeaves, treeChars);
    EXPECT_EQUAL(file.messageBits, messageBits);
}


STUDENT_TEST("Compress undoes decompress on a range of strings.") {
    Vector<string> testCases = {
        "$￥￥￥￥￥，。。",
        "aA877777^^……",
        "おはよう御座います",
        "你好"
    };
    for (string test: testCases) {
        HuffmanResult file = compress(test);
        string result = decompress(file);

        EXPECT_EQUAL(result.size(), test.size());
        EXPECT_EQUAL(test, result);
    }
}

/* * * * * Provided Tests Below This Point * * * * */
#include <limits>

/* Utility function to test if a purported Huffman tree is indeed a Huffman tree.
 * Specifically, this checks that each internal node has either zero or two
 * children. There are other ways you could produce an invalid Huffman tree - for
 * example, by having uninitialized pointers or by linking in a cycle - but we
 * don't test for that here.
 */
bool isEncodingTree(EncodingTreeNode* tree) {
    /* The empty tree is not a Huffman tree. */
    if (tree == nullptr) return false;

    /* If we have one missing child, we should have two missing children. */
    if ((tree->zero == nullptr) != (tree->one == nullptr)) return false;

    /* If we have children at all, they need to be Huffman trees. */
    return tree->zero == nullptr || (isEncodingTree(tree->zero) && isEncodingTree(tree->one));
}

/* Utility function to test if two trees are equal. This is adapted from Section
 * Handout 8 and particularized to Huffman trees.
 */
bool areEqual(EncodingTreeNode* lhs, EncodingTreeNode* rhs) {
    /* Base case: If either is a leaf, both should be. */
    bool lhsLeaf = lhs->zero == nullptr && lhs->one == nullptr;
    bool rhsLeaf = rhs->zero == nullptr && rhs->one == nullptr;
    if (lhsLeaf || rhsLeaf) {
        return lhs->ch == rhs->ch && lhsLeaf == rhsLeaf;
    }

    /* Otherwise, they're both internal nodes. Check that they match. */
    return areEqual(lhs->zero, rhs->zero) && areEqual(lhs->one, rhs->one);
}

/* Utility function to return a string of all possible characters. */
string pangrammaticString() {
    string result;

    char ch = numeric_limits<char>::min();
    result += ch;
    do {
        ch++;
        result += ch;
    } while (ch != numeric_limits<char>::max());

    return result;
}

/* Utility function that makes an inefficient but still valid encoding tree
 * for the given characters.
 */
EncodingTreeNode* strandTreeFor(const string& text, size_t index = 0) {
    if (index == text.size()) error("No characters provided to strandTreeFor.");

    /* We always get a leaf node. */
    EncodingTreeNode* leaf = new EncodingTreeNode {
            text[index], nullptr, nullptr
};

    /* Last character? If so, that's all. */
    if (index + 1 == text.size()) return leaf;

    /* Otherwise, build a larger tree. */
    else return new EncodingTreeNode {
        ' ', leaf, strandTreeFor(text, index + 1)
    };
}

PROVIDED_TEST("huffmanTreeFor reports errors on invalid inputs.") {
    EXPECT_ERROR(huffmanTreeFor(""));    // No characters
    EXPECT_ERROR(huffmanTreeFor("a"));   // One character
    EXPECT_ERROR(huffmanTreeFor("aaa")); // One character
}

PROVIDED_TEST("huffmanTreeFor builds tree for two characters.") {
    EncodingTreeNode* reference = new EncodingTreeNode {
            ' ', new EncodingTreeNode {'a', nullptr, nullptr}, new EncodingTreeNode {'b', nullptr, nullptr}
};

    EncodingTreeNode* tree = huffmanTreeFor("aaabbbb");
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

PROVIDED_TEST("huffmanTreeFor works on the full range of characters.") {
    /* Get a string of all possible characters, then pair them off and see what we find. */
    string allChars = pangrammaticString();
    for (size_t i = 0; i < allChars.size(); i += 2) {
        string toEncode;
        toEncode += allChars[i];
        toEncode += allChars[i + 1];
        toEncode += allChars[i + 1];

        EncodingTreeNode* reference = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };

    EncodingTreeNode* tree = huffmanTreeFor(toEncode);
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}
}

PROVIDED_TEST("huffmanTreeFor uses cumulative weights (v1).") {
    /* This tree:
     *                 *
     *                / \
     *               *   D
     *              / \
     *             C   *
     *                / \
     *               A   B
     */
    EncodingTreeNode* reference = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { '!',
            new EncodingTreeNode { 'C', nullptr, nullptr },
            new EncodingTreeNode { '?',
            new EncodingTreeNode { 'A', nullptr, nullptr },
            new EncodingTreeNode { 'B', nullptr, nullptr }
}
},
            new EncodingTreeNode { 'D', nullptr, nullptr }
};

    /* Ax2, Bx3, Cx4, Dx10 */
    EncodingTreeNode* tree = huffmanTreeFor("AABBBCCCCDDDDDDDDDD");
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

PROVIDED_TEST("huffmanTreeFor uses cumulative weights (v2).") {
    /*
     *          *
     *       /     \
     *      *       *
     *     / \     / \
     *    D   E   F   *
     *               / \
     *              C   *
     *                 / \
     *                A   B
     */
    EncodingTreeNode* reference =new EncodingTreeNode {
            ' ',
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'D', nullptr, nullptr },
            new EncodingTreeNode{ 'E', nullptr, nullptr }
},
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'F', nullptr, nullptr },
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'C', nullptr, nullptr },
            new EncodingTreeNode{
            ' ',
            new EncodingTreeNode{ 'A', nullptr, nullptr },
            new EncodingTreeNode{ 'B', nullptr, nullptr },
}
}
}
};

    /* Ax2, Bx3, Cx4, Dx6, Ex7, Fx8 */
    EncodingTreeNode* tree = huffmanTreeFor("AABBBCCCCDDDDDDEEEEEEEFFFFFFFF");
    EXPECT(isEncodingTree(tree));

    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

PROVIDED_TEST("decodeText works on small sample.") {
    /* This tree:
     *                 *
     *                / \
     *               O   *
     *                  / \
     *                 *   N
     *                / \
     *               M   S
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { 'O', nullptr, nullptr },
            new EncodingTreeNode { '*',
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'M', nullptr, nullptr },
            new EncodingTreeNode { 'S', nullptr, nullptr }
},
            new EncodingTreeNode{ 'N', nullptr, nullptr }
}
};

    /* What you get if you encode MONSOON with this tree. */
    Queue<Bit> bits = { 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1 };

    EXPECT_EQUAL(decodeText(bits, tree), "MONSOON");

    deleteTree(tree);
}

PROVIDED_TEST("Can decode all char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try decoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        string expected;
        expected += allChars[i];
        expected += allChars[i + 1];
        expected += allChars[i + 1];

        EncodingTreeNode* tree = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };

    /* Decode the bitstream 011, which should map back to the expected
         * string.
         */
    Queue<Bit> bits = { 0, 1, 1 };
    EXPECT_EQUAL(decodeText(bits, tree), expected);

    deleteTree(tree);
}
}

PROVIDED_TEST("encodeText works on small sample.") {
    /* This tree:
     *                 *
     *                / \
     *               O   *
     *                  / \
     *                 *   N
     *                / \
     *               M   S
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { 'O', nullptr, nullptr },
            new EncodingTreeNode { '*',
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'M', nullptr, nullptr },
            new EncodingTreeNode { 'S', nullptr, nullptr }
},
            new EncodingTreeNode{ 'N', nullptr, nullptr }
}
};

    /* What you get if you encode MONSOON with this tree. */
    Queue<Bit> expected = { 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1 };

    EXPECT_EQUAL(encodeText("MONSOON", tree), expected);

    deleteTree(tree);
}

PROVIDED_TEST("Can encode all char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try encoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        string toEncode;
        toEncode += allChars[i];
        toEncode += allChars[i + 1];
        toEncode += allChars[i + 1];

        EncodingTreeNode* tree = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };

    /* See what bits we get back. We should get 011, since the first
         * character has code 0 and the second has code 1.
         */
    Queue<Bit> bits = encodeText(toEncode, tree);
    Queue<Bit> expected = { 0, 1, 1 };

    EXPECT_EQUAL(bits, expected);

    deleteTree(tree);
}
}

PROVIDED_TEST("decodeText undoes encodeText on range of sample strings.") {
    Vector<string> testCases = {
        "THAT THAT IS IS THAT THAT IS NOT IS NOT IS THAT IT IT IS",
        "AABAAABBABAAABAAAA",
        ":-) :-D XD <(^_^)>",
        pangrammaticString(),
    };

    for (string test: testCases) {
        /* Use a silly encoding scheme, but one that works regardless of what
         * characters are provided.
         */
        EncodingTreeNode* tree = strandTreeFor(test);
        EXPECT(isEncodingTree(tree));

        Queue<Bit> bits = encodeText(test, tree);
        string result = decodeText(bits, tree);

        EXPECT_EQUAL(test.size(), result.size());
        EXPECT_EQUAL(test, result);

        deleteTree(tree);
    }
}

PROVIDED_TEST("Can decode an example tree.") {
    /* This encodes this tree:
     *
     *                 *
     *                / \
     *               *   C
     *              / \
     *             A   B
     */
    Queue<Bit>  bits   = { 1, 1, 0, 0, 0 };
    Queue<char> leaves = { 'A', 'B', 'C' };

    EncodingTreeNode* tree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(tree));

    /* Confirm this is the right tree. */
    EncodingTreeNode* expected = new EncodingTreeNode {
            '*',
            new EncodingTreeNode {
            '*',
            new EncodingTreeNode { 'A', nullptr, nullptr },
            new EncodingTreeNode { 'B', nullptr, nullptr },
},
            new EncodingTreeNode { 'C', nullptr, nullptr }
};

    EXPECT(areEqual(tree, expected));

    deleteTree(tree);
    deleteTree(expected);
}

PROVIDED_TEST("Can decode trees using all possible char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try encoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        EncodingTreeNode* expected = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };
    Queue<Bit>  treeBits   = { 1, 0, 0 };
    Queue<char> treeLeaves = { allChars[i], allChars[i + 1] };

    EncodingTreeNode* tree = decodeTree(treeBits, treeLeaves);
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, expected));

    deleteTree(tree);
    deleteTree(expected);
}
}

PROVIDED_TEST("Can encode an example tree.") {
    /* Build an encoding tree for "ABBCCCC." It should look like this:
     *
     *                 *
     *                / \
     *               *   C
     *              / \
     *             A   B
     *
     * This will compress down to
     *
     *        11000
     *        ABC
     */
    EncodingTreeNode* tree = huffmanTreeFor("ABBCCCC");

    Queue<Bit>  bits;
    Queue<char> leaves;

    encodeTree(tree, bits, leaves);

    Queue<Bit>  expectedBits   = { 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'A', 'B', 'C' };

    EXPECT_EQUAL(bits,   expectedBits);
    EXPECT_EQUAL(leaves, expectedLeaves);

    deleteTree(tree);
}

PROVIDED_TEST("Can encode trees using all possible char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try encoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        EncodingTreeNode* tree = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };

    /* See what we get back. It should be the bitstring 100 (root with
         * two children) and the two leaves, in order.
         */
    Queue<Bit>  treeBits;
    Queue<char> treeLeaves;

    Queue<Bit>  expectedBits = { 1, 0, 0 };
    Queue<char> expectedLeaves = { allChars[i], allChars[i + 1] };

    encodeTree(tree, treeBits, treeLeaves);
    EXPECT_EQUAL(treeBits, expectedBits);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deleteTree(tree);
}
}

PROVIDED_TEST("decodeTree undoes encodeTree on sample strings.") {
    /* Make a Huffman tree for the string of all characters. */
    EncodingTreeNode* sourceTree = huffmanTreeFor(pangrammaticString());
    EXPECT(isEncodingTree(sourceTree));

    /* Encode, then decode it. */
    Queue<Bit>  bits;
    Queue<char> leaves;
    encodeTree(sourceTree, bits, leaves);

    EncodingTreeNode* resultTree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(resultTree));
    EXPECT(areEqual(sourceTree, resultTree));

    deleteTree(sourceTree);
    deleteTree(resultTree);
}

PROVIDED_TEST("Can decompress a small sample file.") {
    HuffmanResult file = {
        { 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
        { 'u', 'k', 'p', 'n', 'a', 'm', 'h' },
        { 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1,
          0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
          0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0 }
    };

    EXPECT_EQUAL(decompress(file), "humuhumunukunukuapuaa");
}

PROVIDED_TEST("Compress reports errors on bad inputs.") {
    EXPECT_ERROR(compress(""));
    EXPECT_ERROR(compress("A"));
    EXPECT_ERROR(compress("AAAA"));
}

PROVIDED_TEST("Can compress a small sample file.") {
    HuffmanResult file = compress("ABANANAABANDANA");
    Queue<Bit>  treeBits    = { 1, 1, 1, 0, 0, 0, 0 };
    Queue<char> treeChars   = { 'D', 'B', 'N', 'A' };
    Queue<Bit>  messageBits = { 1, 0, 0, 1, 1, 0, 1, 1, 0,
                                1, 1, 1, 0, 0, 1, 1, 0, 1,
                                0, 0, 0, 1, 0, 1, 1 };

    EXPECT_EQUAL(file.treeBits, treeBits);
    EXPECT_EQUAL(file.treeLeaves, treeChars);
    EXPECT_EQUAL(file.messageBits, messageBits);
}

PROVIDED_TEST("Compress undoes decompress on a range of strings.") {
    Vector<string> testCases = {
        "THAT THAT IS IS THAT THAT IS NOT IS NOT IS THAT IT IT IS",
        "AABAAABBABAAABAAAA",
        ":-) :-D XD <(^_^)>",
        pangrammaticString(),
    };
    for (string test: testCases) {
        HuffmanResult file = compress(test);
        string result = decompress(file);

        EXPECT_EQUAL(result.size(), test.size());
        EXPECT_EQUAL(test, result);
    }
}

