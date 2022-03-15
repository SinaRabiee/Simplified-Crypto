#include "client.h"

Client::Client(std::string id, const Server& server)
    : id { id }
    , server { &server }
{
    crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() const
{
    return id;
}

std::string Client::get_publickey() const
{
    return public_key;
}

double Client::get_wallet() const
{
    auto cnstS { this->server };
    Server S { *cnstS };
    return S.get_wallet(id);
}

std::string Client::sign(std::string txt) const
{
    return crypto::signMessage(private_key, txt);
}

bool Client::transfer_money(std::string receiver, double value)
{
    if (this->get_wallet() >= value) {
        auto cnstS { this->server };
        Server S { *cnstS };
        if (S.get_client(receiver) != nullptr) {
            std::string trx {};
            trx = id + "-" + receiver + "-" + std::to_string(value);
            std::string strx { sign(trx) };
            S.add_pending_trx(trx, strx);
            return true;
        } else {
            std::cout << "There is no client with this ID" << std::endl;
            return false;
        }
    } else {
        std::cout << "There is not enough coins in the wallet" << std::endl;
        return false;
    }
}

size_t Client::generate_nonce()
{
    size_t max { std::numeric_limits<size_t>::max() };
    std::random_device seeder;
    std::default_random_engine generator(seeder());
    std::uniform_int_distribution<> distribution(0, max);
    return static_cast<size_t>(distribution(generator));
}