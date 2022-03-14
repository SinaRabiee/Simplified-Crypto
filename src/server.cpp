#include "server.h"

std::shared_ptr<Client> Server::add_client(std::string id)
{
    size_t flag {};
    for (auto name { clients.begin() }; name != clients.end(); name++) {
        if (((name->first)->get_id()).compare(id) == 0) {
            flag = 1;
            break;
        }
    }

    std::shared_ptr<Client> C { nullptr };
    if (flag == 1) {
        std::string num {};
        std::random_device seeder;
        std::default_random_engine generator(seeder());
        std::uniform_int_distribution<> distribution(0, 9999);
        num = std::to_string(distribution(generator));
        C = std::make_shared<Client>(id + num, *this);
    } else {
        C = std::make_shared<Client>(id, *this);
    }
    clients[C] = 5.0;
    return C;
}

std::shared_ptr<Client> Server::get_client(std::string id) const
{
    size_t flag {};
    std::shared_ptr<Client> C { nullptr };

    for (auto name { clients.begin() }; name != clients.end(); name++) {
        if (((name->first)->get_id()).compare(id) == 0) {
            C = name->first;
            flag = 1;
            break;
        }
    }
    if (flag == 1)
        return C;
    else
        return nullptr;
}

double Server::get_wallet(std::string id) const
{
    double money {};
    size_t flag {};

    for (auto name { clients.begin() }; name != clients.end(); name++) {
        if (((name->first)->get_id()).compare(id) == 0) {
            money = name->second;
            flag = 1;
            break;
        }
    }
    if (flag == 1)
        return money;
    else {
        throw std::runtime_error("There is no client with this ID");
    }
}

bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value)
{
    int count {};
    for (size_t i {}; i <= trx.length() - 1;) {
        size_t position = trx.find("-", i);
        if (position == std::string::npos)
            break;
        ++count;
        i = position + 1;
    }

    if (count == 2) {
        size_t p1 = trx.find("-");
        size_t p2 = trx.find("-", p1 + 1);
        std::string send { trx.substr(0, p1) };
        std::string receive { trx.substr(p1 + 1, p2 - p1 - 1) };
        double val { std::stod(trx.substr(p2 + 1)) };
        sender = send;
        receiver = receive;
        value = val;
        return true;
    } else
        throw std::runtime_error("String is not standard");
}

bool Server::add_pending_trx(std::string trx, std::string signature)
{
    std::string send {};
    std::string receive {};
    double val {};
    parse_trx(trx, send, receive, val);
    std::string senderSign { get_client(send)->get_publickey() };
    bool authentic { crypto::verifySignature(senderSign, trx, signature) };
    if (this->get_wallet(send) >= val) {
        if (authentic) {
            pending_trxs.push_back(trx);
            return true;
        } else {
            std::cout << "signature is not valid" << std::endl;
            return false;
        }
    } else {
        std::cout << "There is not enough coins in the wallet" << std::endl;
        return false;
    }
}

size_t Server::mine()
{
    std::string mempool {};
    for (const auto& ptrxs : pending_trxs) {
        mempool += ptrxs;
    }

    while (true) {
        for (auto name { clients.begin() }; name != clients.end(); name++) {
            size_t nonce { (name->first)->generate_nonce() };
            std::string nonceid { (name->first)->get_id() };
            std::string hash { crypto::sha256(mempool + std::to_string(nonce)).substr(0, 10) };
            if (hash.find("000") != std::string::npos) {
                std::string sender {};
                std::string receiver {};
                double value {};
                std::cout << "Mined by " << nonceid << std::endl;
                clients[get_client(nonceid)] += 6.25;
                for (const auto& trx : pending_trxs) {
                    parse_trx(trx, sender, receiver, value);
                    auto senderC { get_client(sender) };
                    auto receiverC { get_client(receiver) };
                    clients[senderC] -= value;
                    clients[receiverC] += value;
                }
                pending_trxs.clear();
                return nonce;
            }
        }
    }
}
