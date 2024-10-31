#ifndef BASHCOMPILER_H
#define BASHCOMPILER_H

/***********************************************
# ==============================================
# | Author: Taylo @ https://github.com/i-Taylo |
# ==============================================
*///////////////////////////////////////////////

std::string part1 = R"(
	#include <iostream>
	#include <fstream>
	#include <string>
	#include <cstdlib>
	#include <unistd.h>
	#include <sys/stat.h>
	#include <signal.h>
	#include <random>
	#include <limits.h>  // for PATH_MAX
	#include <libgen.h>  // for dirname
	#include <unistd.h>  // for readlink
        
	namespace ay {
    	using size_type = unsigned long long;
    	using key_type = uint64_t;  // Explicitly use uint64_t

    	template <typename T>
    	struct remove_const_ref {
        	using type = T;
    	};

    	template <typename T>
    	struct remove_const_ref<T&> {
        	using type = T;
    	};

    	template <typename T>
    	struct remove_const_ref<const T> {
        	using type = T;
    	};

    	template <typename T>
    	struct remove_const_ref<const T&> {
        	using type = T;
    	};

    	template <typename T>
    	using char_type = typename remove_const_ref<T>::type;

    	constexpr key_type generate_key(key_type seed)
    	{
        	key_type key = seed;
        	key ^= (key >> 33);
        	key *= 0xff51afd7ed558ccdULL;
        	key ^= (key >> 33);
        	key *= 0xc4ceb9fe1a85ec53ULL;
        	key ^= (key >> 33);
        	key |= 0x0101010101010101ULL;
        	return key;
    	}

    	template <typename CHAR_TYPE>
    	constexpr void cipher(CHAR_TYPE* data, size_type size, key_type key)
    	{
        	for (size_type i = 0; i < size; i++)
        	{
            	data[i] ^= CHAR_TYPE((key >> ((i % 8) * 8)) & 0xFF);
        	}
    	}

    	template <size_type N, key_type KEY, typename CHAR_TYPE = char>
    	class obfuscator
    	{
    	public:
        	constexpr obfuscator(const CHAR_TYPE* data)
        	{
            	for (size_type i = 0; i < N; i++)
            	{
                	m_data[i] = data[i];
            	}
            	cipher(m_data, N, KEY);
        	}

        	constexpr const CHAR_TYPE* data() const
        	{
            	return &m_data[0];
        	}

        	constexpr size_type size() const
        	{
            	return N;
        	}

        	constexpr key_type key() const
        	{
            	return KEY;
        	}

    	private:
        	CHAR_TYPE m_data[N]{};
    	};

    	template <size_type N, key_type KEY, typename CHAR_TYPE = char>
    	class obfuscated_data
    	{
    	public:
        	obfuscated_data(const obfuscator<N, KEY, CHAR_TYPE>& obfuscator)
        	{
            	for (size_type i = 0; i < N; i++)
            	{
                	m_data[i] = obfuscator.data()[i];
            	}
        	}

        	~obfuscated_data()
        	{
            	for (size_type i = 0; i < N; i++)
            	{
                	m_data[i] = 0;
            	}
        	}

        	operator CHAR_TYPE* ()
        	{
            	decrypt();
            	return m_data;
        	}

        	void decrypt()
        	{
            	if (m_encrypted)
            	{
                	cipher(m_data, N, KEY);
                	m_encrypted = false;
            	}
        	}

        	void encrypt()
        	{
            	if (!m_encrypted)
            	{
                	cipher(m_data, N, KEY);
                	m_encrypted = true;
            	}
        	}

	        bool is_encrypted() const
        	{
            	return m_encrypted;
        	}

    	private:
        	CHAR_TYPE m_data[N];
        	bool m_encrypted{ true };
    	};

    	template <size_type N, key_type KEY, typename CHAR_TYPE = char>
    	constexpr auto make_obfuscator(const CHAR_TYPE(&data)[N])
    	{
        	return obfuscator<N, KEY, CHAR_TYPE>(data);
    	}
	}

	#define AY_OBFUSCATE_KEY(data, key) \
    	[]() -> ay::obfuscated_data<sizeof(data)/sizeof(data[0]), key, ay::char_type<decltype(*data)>>& { \
        	static_assert(sizeof(key) == sizeof(ay::key_type), "key must be a 64 bit unsigned integer"); \
        	static_assert((key) >= (1ull << 56), "key must span all 8 bytes"); \
        	using char_type = ay::char_type<decltype(*data)>; \
        	constexpr auto n = sizeof(data)/sizeof(data[0]); \
        	constexpr auto obfuscator = ay::make_obfuscator<n, key, char_type>(data); \
        	thread_local auto obfuscated_data = ay::obfuscated_data<n, key, char_type>(obfuscator); \
        	return obfuscated_data; \
    	}()

	namespace {
    	volatile sig_atomic_t g_running = 1;
    	void signal_handler(int) {
        	g_running = 0;
    	}
	}

	class ScriptExecutor {
	public:

	static std::string getExecutablePath(const char* argv0 = nullptr) {
    	char result[PATH_MAX];
    
    	#if defined(__linux__)
    	ssize_t count = readlink("/proc/self/exe", result, sizeof(result) - 1);
    	if (count != -1) {
        	result[count] = '\0';
        	return std::string(dirname(result));
    	}
    	#endif

    	if (argv0) {
        	char* resolved = realpath(argv0, result);
        	if (resolved) {
            	return std::string(dirname(resolved));
        	}
    	}

    	if (getcwd(result, sizeof(result)) != nullptr) {
        	return std::string(result);
    	}
    	return ".";
	}

    static void execute(int argc, char* argv[]) {
        // Set up signal handling
        struct sigaction sa;
        sa.sa_handler = signal_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, nullptr);
        sigaction(SIGTERM, &sa, nullptr);

        // Get obfuscated script)";

        std::string part2 = "\n        const uint64_t scriptKey = " + std::to_string(scriptKey) + "ULL;\n";
        std::string part3 = "        auto& script = AY_OBFUSCATE_KEY(\"" + escapedScript + "\", scriptKey);\n";
        
        std::string part4 = R"(
        // Get executable directory
        std::string execDir = getExecutablePath(argv[0]);
        
        // Generate random filename
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 35);
        const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyz";
        std::string random_suffix;
        for(int i = 0; i < 10; i++) {
            random_suffix += charset[dis(gen)];
        }
        
        std::string tempname = execDir + "/.script_" + random_suffix;
        
        {
            std::ofstream temp_script(tempname);
            if (!temp_script) {
                throw std::runtime_error("Failed to create temporary file");
            }
            temp_script << "#!/bin/bash\n";
            temp_script << static_cast<char*>(script);
            temp_script.close();
        }
        
        if (chmod(tempname.c_str(), 0755) != 0) {
            unlink(tempname.c_str());
            throw std::runtime_error("Failed to set executable permissions");
        }
        
        std::string command = "bash \"" + tempname + "\"";
        for (int i = 1; i < argc; ++i) {
            command += " \"" + std::string(argv[i]) + "\"";
        }
        
        int result = system(command.c_str());
        
        unlink(tempname.c_str());
        
        if (WIFEXITED(result)) {
            int exit_status = WEXITSTATUS(result);
            if (exit_status != 0) {
                throw std::runtime_error("Script execution failed with exit code: " + std::to_string(exit_status));
            }
        } else {
            throw std::runtime_error("Script execution failed abnormally");
        }
    }
};

int main(int argc, char* argv[]) {
    try {
        ScriptExecutor::execute(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
})";


#endif // BASHCOMPILER_H