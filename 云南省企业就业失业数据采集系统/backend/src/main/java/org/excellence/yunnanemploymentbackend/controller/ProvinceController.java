package org.excellence.yunnanemploymentbackend.controller;

import org.excellence.yunnanemploymentbackend.entity.EnterpriseInfo;
import org.excellence.yunnanemploymentbackend.entity.Notice;
import org.excellence.yunnanemploymentbackend.entity.ReportedData;
import org.excellence.yunnanemploymentbackend.entity.Response;
import org.excellence.yunnanemploymentbackend.service.ProvinceService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.bind.annotation.CrossOrigin;

import java.util.ArrayList;
import java.util.List;

@RequestMapping(path = "province")
@RestController
@CrossOrigin("*")
public class ProvinceController {
    @Autowired
    private ProvinceService provinceService;

    /**
     * 查看所有已备案企业信息
     * @return 所有已备案企业的列表
     */
    @GetMapping(path = "get-all-registered-enterprise")
    public @ResponseBody Response<List<EnterpriseInfo>> getAllRegisteredEnterprise() {
        try {
            return new Response<>(true, "success", provinceService.getAllRegisteredEnterprise());
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), new ArrayList<>());
        }
    }

    /**
     * 查看所有已备案企业信息
     * @return 所有已备案企业的列表
     */
    @GetMapping(path = "get-all-enterprise")
    public @ResponseBody Response<List<EnterpriseInfo>> getAllEnterprise() {
        try {
            return new Response<>(true, "success", provinceService.getAllEnterprise());
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), new ArrayList<>());
        }
    }

    /**
     * 查看各市已备案企业信息
     * @param cityUserId 所查询的市的userId
     * @return 当前市已备案的所有企业列表
     */
    @GetMapping(path = "get-all-registered-enterprise/{city-userid}")
    public @ResponseBody Response<List<EnterpriseInfo>> getRegisteredEnterpriseIn(@PathVariable("city-userid") String cityUserId) {
        try {
            return new Response<>(true, "success", provinceService.getRegisteredEnterpriseIn(cityUserId));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), new ArrayList<>());
        }
    }

    /**
     * 获取所有待审核的企业上报数据
     * @return 待审核的企业上报数据的列表
     * @param provinceUserId 省的userId(方便多省扩展)
     */
    @GetMapping(path = "get-all-reported-data-awaiting-review/{province-userid}")
    public @ResponseBody Response<List<ReportedData>> getAllReportedDataAwaitingReview(@PathVariable("province-userid") String provinceUserId) {
        try {
            return new Response<>(true, "success", provinceService.getAllReportedDataAwaitingReview(provinceUserId));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), new ArrayList<>());
        }
    }

    /**
     * 省审核通过上报的数据
     * @param reportedDataId 上报数据的Id
     * @return 操作是否成功
     */
    @GetMapping(path = "approve-reported-data/{province-userid}/{reported-data-id}")
    public @ResponseBody Response<Boolean> approveReportedData(@PathVariable("reported-data-id") Integer reportedDataId, @PathVariable("province-userid") String provinceUserId) {
        try {
            return new Response<>(true, "success", provinceService.approveReportedData(provinceUserId, reportedDataId));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), false);
        }
    }

    /**
     * 省拒绝通过企业上报的数据
     * @param provinceUserId 省的UserId
     * @param reportedDataId 上报数据的Id
     * @return 操作是否成功
     */
    @GetMapping(path = "reject-reported-data/{province-userid}/{reported-data-id}")
    public @ResponseBody Response<Boolean> rejectReportedData(@PathVariable("reported-data-id") Integer reportedDataId, @PathVariable("province-userid") String provinceUserId) {
        try {
            return new Response<>(true, "success", provinceService.rejectReportedData(provinceUserId, reportedDataId));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), false);
        }
    }

    /**
     * 审核通过企业的备案信息
     * @param provinceUserId 省的UserId
     * @param enterpriseInfoId 备案信息的Id
     * @return 操作是否成功
     */
    @GetMapping(path = "approve-enterprise-info/{province-userid}/{enterprise-info-id}")
    public @ResponseBody Response<Boolean> approveEnterpriseInfo(@PathVariable("enterprise-info-id") Integer enterpriseInfoId, @PathVariable("province-userid") String provinceUserId) {
        try {
            return new Response<>(true, "success", provinceService.approveEnterpriseInfo(provinceUserId, enterpriseInfoId));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), false);
        }
    }

    /**
     * 拒绝通过企业的备案信息
     * @param provinceUserId 省的UserId
     * @param enterpriseInfoId 备案信息的Id
     * @return 操作是否成功
     */
    @GetMapping(path = "reject-enterprise-info/{province-userid}/{enterprise-info-id}")
    public @ResponseBody Response<Boolean> rejectEnterpriseInfo(@PathVariable("enterprise-info-id") Integer enterpriseInfoId, @PathVariable("province-userid") String provinceUserId) {
        try {
            return new Response<>(true, "success", provinceService.rejectEnterpriseInfo(provinceUserId, enterpriseInfoId));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), false);
        }
    }

    /**
     * 修改企业的上报数据
     * @param reportedData 修改之后的上报数据
     * @return 操作是否成功
     */
    @PostMapping(path = "modify-reported-data")
    public @ResponseBody Response<Boolean> modifyReportedData(@RequestBody ReportedData reportedData) {
        try {
            return new Response<>(true, "success", provinceService.modifyReportedData(reportedData));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), false);
        }
    }

    /**
     * 发布通知
     * @param provinceUserId 省用户的UserId
     * @param notice 具体通知内容
     * @return 操作是否成功
     */
    @PostMapping(path = "publish-notice/{province-userid}")
    public @ResponseBody Response<Boolean> publishNotice(@PathVariable("province-userid") String provinceUserId, @RequestBody Notice notice) {
        try {
            return new Response<>(true, "success", provinceService.releaseNotice(provinceUserId, notice));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), false);
        }
    }

    @GetMapping(path = "show-all-notice")
    public @ResponseBody Response<List<Notice>> getAllNotice() {
        try{
            return new Response<>(true, "success", provinceService.getAllNotice());
        } catch (Exception e){
            return new Response<>(false, e.getMessage(), null);
        }
    }

    @GetMapping(path = "get-notice/{notice-id}")
    public @ResponseBody Response<Notice> getNotice(@PathVariable("notice-id") Integer noticeId) {
        try{
            return new Response<>(true,"success",provinceService.getNotice(noticeId));
        } catch(Exception e) {
            return new Response<>(false, e.getMessage(), null);
        }
    }


}
