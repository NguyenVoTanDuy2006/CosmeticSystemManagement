# Cosmetic Warehouse Management System (LSNX)

A comprehensive desktop application for managing cosmetic products and trade operations in warehouse environments. Built with C++ and Qt6, this system provides a modern graphical interface for efficient inventory and trade management.

## 🌟 Features

### Product Management
- **Product Catalog**: Add, edit, and remove cosmetic products
- **Inventory Tracking**: Real-time stock level monitoring
- **Product Categories**: Support for various cosmetic types (skincare, makeup, fragrance, etc.)
- **Unit Management**: Multiple unit types (bottles, tubes, boxes)
- **Batch/Lot Management**: Track product shipments and expiration dates
- **Image Support**: Product photos and visual catalog

### Trade Management
- **Import Transactions**: Record incoming product shipments
- **Export Transactions**: Track outgoing sales and distributions
- **Client Management**: Customer database and trade history
- **Financial Tracking**: Revenue and cost analysis
- **Trade History**: Complete transaction logs with timestamps
- **Filtering System**: Advanced search and filter capabilities

### User Interface
- **Modern Design**: Clean and intuitive Qt6-based interface
- **Secure Login**: Authentication system with user management
- **Multi-Window Navigation**: Seamless switching between modules
- **Card-Based Layout**: Visual product and trade cards
- **Responsive Design**: Adaptable to different screen sizes

## 🚀 Technology Stack

- **Frontend**: Qt6 Widgets with custom UI components
- **Backend**: C++23 with modern STL features
- **Build System**: CMake 3.16+
- **Data Storage**: File-based storage system
- **Architecture**: Singleton pattern with modular design

## 📋 System Requirements

- **Operating System**: Windows/Linux/macOS
- **Compiler**: C++23 compatible compiler (GCC 11+, Clang 13+, MSVC 2022+)
- **Qt Version**: Qt6.0 or higher
- **CMake**: Version 3.16 or higher
- **RAM**: Minimum 4GB recommended
- **Storage**: 100MB+ for application and data

## 🛠️ Installation & Setup

### Prerequisites
1. Install Qt6 development environment
2. Install CMake (3.16+)
3. Install a C++23 compatible compiler

### Building from Source
```bash
# Clone the repository
git clone <repository-url>
cd CosmeticSystemManagement-finalUI

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the application
cmake --build .

# Run the application
./LSNX  # On Linux/macOS
# or
LSNX.exe  # On Windows
```

### Quick Start
1. Run the application executable
2. Login with your credentials
3. Navigate through the main menu to access different modules
4. Start by adding products to your inventory
5. Record trade transactions as needed

## 📁 Project Structure

```
CosmeticSystemManagement-finalUI/
├── BACKEND/                    # Core business logic
│   ├── Cosmetic/              # Product-related classes
│   ├── CosmeticManagement/    # Product management system
│   ├── Trade/                 # Trade transaction classes
│   ├── TradeManagement/       # Trade management system
│   └── ManageSystem/          # System utilities
├── LOGIN+TRANGCHU/            # Authentication and main menu UI
├── Data/                      # Application data storage
│   └── ProductData/           # Product information and images
├── icon/                      # Application icons and resources
├── *.ui                       # Qt Designer UI files
├── *.cpp/*.h                  # Source and header files
└── CMakeLists.txt            # Build configuration
```

## 🔧 Configuration

### Data Storage
The application stores data in the `Data/` directory:
- **ProductData/**: Product information and images
- **TradeData/**: Transaction records and history
- **Config/**: Application settings (if applicable)

### Customization
- Product categories and types can be modified in `productInfo.hpp`
- UI themes and styles are defined in Qt stylesheets
- Database paths are configurable through the build system

## 📊 Core Modules

### Product Manager
- Singleton pattern for centralized product management
- Support for filtering and searching products
- Import/Export functionality for inventory updates
- Image management for product photos

### Trade Manager
- Handle both import and export transactions
- Client relationship management
- Financial tracking and reporting
- Historical data analysis

### User Interface Components
- **MainMenu**: Central navigation hub
- **SP Module**: Product management interface
- **LSNX Module**: Trade management interface
- **Login System**: Secure authentication

## 🔒 Security Features

- User authentication system
- Secure login with validation
- Data integrity checks
- File access protection

## 🐛 Troubleshooting

### Common Issues
1. **Build Errors**: Ensure Qt6 is properly installed and in PATH
2. **Missing Dependencies**: Check CMake configuration for required libraries
3. **Data Access**: Verify Data/ directory permissions
4. **UI Issues**: Confirm Qt Designer files (.ui) are properly configured

### Support
For technical support and bug reports, please check the project documentation or contact the development team.

## 📝 License

This project is developed for educational and commercial use. Please refer to the license file for detailed terms and conditions.

## 🤝 Contributing

We welcome contributions to improve the system. Please follow these guidelines:
1. Fork the repository
2. Create a feature branch
3. Make your changes with proper documentation
4. Submit a pull request with detailed description

## 📈 Future Enhancements

- Database integration (MySQL/PostgreSQL support)
- Web-based interface
- Mobile application companion
- Advanced reporting and analytics
- Multi-language support
- Cloud synchronization
- Barcode scanning integration

## 👥 Development Team

This application was developed as part of a cosmetic warehouse management solution, combining modern C++ practices with intuitive user interface design.

---

*For more information about specific features or technical details, please refer to the source code documentation or contact the development team.*
