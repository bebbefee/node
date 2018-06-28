function require_ex(file)
    package.loaded[file] = nil; 
    return require(file); 
end

-- 初始化环境
package.path = package.path .. ";?.lua"
