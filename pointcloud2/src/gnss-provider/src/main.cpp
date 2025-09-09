// #include <types/conversion.h>

#include "gnss_someip.h"
#include "gpsd_client.h"

auto main(int argc, char **argv) -> int 
{
    rclcpp::init(argc, argv);

    rclcpp::executors::MultiThreadedExecutor executor;
    int data_size = 4096;

    if (argc > 1) {
        try {
            data_size = std::stoi(argv[1]);
        } catch (const std::invalid_argument& ia) {
            std::cerr << "Invalid argument: " << ia.what() << '\n';
            return 1;
        } catch (const std::out_of_range& oor) {
            std::cerr << "Argument out of range: " << oor.what() << '\n';
            return 1;
        }
    }

    auto gnss_someip_reporter_node = std::make_shared<GnssSomeIpReporter<GnssSomeIpProvider>>();
    auto gpsd_client_node = std::make_shared<GpsdClient>(data_size);

    executor.add_node(gnss_someip_reporter_node);
    executor.add_node(gpsd_client_node);

    executor.spin();
    
    rclcpp::shutdown();
    return 0;
}
