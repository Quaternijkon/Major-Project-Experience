package org.excellence.yunnanemploymentbackend.service;

import org.excellence.yunnanemploymentbackend.entity.City;
import org.excellence.yunnanemploymentbackend.entity.Enterprise;
import org.excellence.yunnanemploymentbackend.entity.Province;
import org.excellence.yunnanemploymentbackend.entity.User;
import org.excellence.yunnanemploymentbackend.repository.CityRepository;
import org.excellence.yunnanemploymentbackend.repository.EnterpriseRepository;
import org.excellence.yunnanemploymentbackend.repository.ProvinceRepository;
import org.excellence.yunnanemploymentbackend.repository.UserRepository;
import org.jetbrains.annotations.NotNull;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Objects;
import java.util.Optional;
import java.util.function.Consumer;
import java.util.function.Supplier;

@Service
public class UserService {
    @Autowired
    private UserRepository userRepository;

    @Autowired
    private ProvinceRepository provinceRepository;

    @Autowired
    private EnterpriseRepository enterpriseRepository;

    @Autowired
    private CityRepository cityRepository;

    public Optional<User> login(User user) {
        final var wrappedUser = userRepository.findById(user.getUserId());
        if (wrappedUser.isEmpty()) {
            return Optional.empty();
        }
        final var trueUser = wrappedUser.get();
        if (!trueUser.getPassword().equals(user.getPassword())) {
            return Optional.empty(); // 密码不正确
        }
        return wrappedUser;
//         final StringBuilder result = new StringBuilder();
//        userRepository.findById(user.getUserId()).ifPresentOrElse(trueUser -> {
//            if (trueUser.getPassword().equals(user.getPassword())) {
//                // 密码正确
//                result.append("success");
//            } else {
//                result.append("wrong password");
//            }
//        }, () -> result.append("userId does not exist"));
//        return result.toString();
    }

    public String register(User user) {
        final StringBuilder result = new StringBuilder();
        userRepository.findById(user.getUserId()).ifPresentOrElse(user1 -> result.append("userId has already existed"), () -> {
            userRepository.save(user);
            // 根据不同用户类型执行不同操作
            if (Objects.equals(user.getUserType(), User.UserType.PROVINCE)) {
                final var province = new Province();
                province.setUserId(user.getUserId());
                provinceRepository.save(province);
            } else if (Objects.equals(user.getUserType(), User.UserType.CITY)) {
                final var city = new City();
                city.setUserId(user.getUserId());
                cityRepository.save(city);
            } else {
                final var enterprise = new Enterprise();
                enterprise.setUserId(user.getUserId());
                enterpriseRepository.save(enterprise);
            }
            result.append("success");
        });
        return result.toString();
    }
    
}
