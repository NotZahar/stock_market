#include <stock_market.hpp>
#include <options_parser.hpp>
#include <logger.hpp>

int main(int argc, char** argv) {
    sm::OptionsParser optionsParser(argc, argv);
    const auto options = optionsParser.getOptions();
    if (options.help) {
        sm::Logger::instance().logHelp();
        return 0;
    }

    sm::StockMarket stockMarket{ std::move(options) };
    stockMarket.run();

    return 0;
}
